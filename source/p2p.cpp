#include "p2p.h"

#include <stdio.h>
#include <string.h> // memset()
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>

#include "logger.h"
#include "../protobuf/client.pb.h"

namespace DBC {

P2P::P2P(){
    mRunning = false;
}

P2P::~P2P(){
    stop();
}

bool P2P::start(){
    std::unique_lock<std::mutex> lock(mThreadMutex);
    std::unique_lock<std::mutex> lock2(mDataMutex);
    Logger::log(LogType::P2P, DEBUG, "Starting P2P network");

    if(mRunning){
        //Logger::log(LogType::P2P, INFO, "Stoping P2P network before starting it");
        //stop();
        return false; //Its ok
    }

    //We first do the initialization of socket in the main thread
    //This make seasy to report errors
    //If everything in the setup succeeds, then start the thread loop

    struct addrinfo hints, *res;
    int reuseaddr = 1;

    // Get the address info
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(NULL, mPort, &hints, &res) != 0) {
        Logger::log(LogType::P2P, ERROR, "getaddrinfo %d", errno);
        return true;
    }

    // Create the socket
    mMainSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (mMainSocket == -1) {
        Logger::log(LogType::P2P, ERROR, "socket");
        return true;
    }

    // Enable the socket to reuse the address
    if (setsockopt(mMainSocket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        Logger::log(LogType::P2P, ERROR, "setsockopt %d", errno);
        return true;
    }

    // Bind to the address
    if (bind(mMainSocket, res->ai_addr, res->ai_addrlen) == -1) {
        Logger::log(LogType::P2P, ERROR, "bind %d", errno);
        return true;
    }

    // Listen
    if (listen(mMainSocket, mListenQueue) == -1) {
        Logger::log(LogType::P2P, ERROR, "listen %d", errno);
        return true;
    }
    freeaddrinfo(res);

    Logger::log(LogType::P2P, INFO, "opened listen socket on port %s", mPort);

    mEventFd = eventfd(0,0);
    if(mEventFd == -1){
        Logger::log(LogType::P2P, ERROR, "Eventfd not set up %d", errno);
        return true;
    }

    //Launch thread
    mClientSocket.clear();
    mThread = std::thread(&P2P::thread_loop, this);
    mRunning = true;

    return false;
}

bool P2P::stop(){
    std::unique_lock<std::mutex> lock(mThreadMutex);
    if(mRunning){
        //Send signal to thread via signalfd
        uint64_t u64 = 1;
        write(mEventFd, &u64, sizeof(uint64_t));
        mThread.join();
        close(mEventFd);
        mEventFd = -1;
    }
    mRunning = false;
}

int P2P::thread_loop(void) {
    Logger::log(LogType::P2P, INFO, "thread created");

    //Inaitialize epoll
    int epfd = epoll_create(1);
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = -1; //-1 is the main socket, not a client socket
    epoll_ctl(epfd, EPOLL_CTL_ADD, mMainSocket, &event);
    event.data.fd = -2; //-2 is the event socket
    epoll_ctl(epfd, EPOLL_CTL_ADD, mEventFd, &event);

    // Main loop
    while (mRunning) {
        if(epoll_wait(epfd, &event, 1, -1) == -1){
            Logger::log(LogType::P2P, ERROR, "epoll error %d", errno);
            break;
        }

        //Exit signal
        if(event.data.fd == -2){
            break;
        }


        std::unique_lock<std::mutex> lock(mDataMutex);

        //Process the socket that has data
        if(event.data.fd == -1){
            //New connection in Main socket
            socklen_t size = sizeof(struct sockaddr_in);
            struct sockaddr_in their_addr;
            int newsock = accept(mMainSocket, (struct sockaddr*)&their_addr, &size);
            if (newsock == -1) {
                Logger::log(LogType::P2P, ERROR, "accept error %d", errno);
            }else{
                Logger::log(LogType::P2P, INFO, "Got a connection (id %d) from %s on port %d",
                        newsock, inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));
                //Add it to the client socket list & epoll
                mClientSocket[newsock] = {their_addr};
                event.events = EPOLLIN;
                event.data.fd = newsock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, newsock, &event);
            }
        }else{
            //Attend this socket
            ClientData& data = mClientSocket[event.data.fd];
            data.rxBuf.resize(data.rxBuf.size()+4096); //Extend it by 4096
            int valread = -1;
            if ((valread = read(event.data.fd, data.rxBuf.data()+data.rxBuf.size()-4096, 4096)) == 0){
                //Somebody disconnected
                Logger::log(LogType::P2P, INFO, "Disconnected (id %d) from %s on port %d",
                        event.data.fd, inet_ntoa(data.addr.sin_addr), htons(data.addr.sin_port));

                //Close the socket and remove from poll
                close(event.data.fd);
                mClientSocket.erase(event.data.fd);
                epoll_ctl(epfd, EPOLL_CTL_DEL, event.data.fd, NULL);
            }else{
                //Packet on already connected client
                Logger::log(LogType::P2P, DEBUG, "Packet on connected client (size %d)", valread);

                data.rxBuf.resize(data.rxBuf.size()-4096+valread); //Shrink it again

                //Process it
                //TODO
                //For the time being just print it
                Logger::log(LogType::P2P, DEBUG, "Client: %s", data.rxBuf.data());
            }
        }
    }

    //Stop epoll
    close(epfd);

    std::unique_lock<std::mutex> lock(mDataMutex);

    //Stop all sockets
    for(auto& i : mClientSocket){
        close(i.first);
    }
    mClientSocket.clear();
    close(mMainSocket);
    mMainSocket = -1;

    Logger::log(LogType::P2P, INFO, "thread stopped");

    return 0;
}

int P2P::getNumClients(){
    std::unique_lock<std::mutex> lock(mDataMutex);

    int num = 0;
    for(auto& i : mClientSocket){
        num += i.second.mHandshake;
    }
    return num;
}

} //namespace DBC
