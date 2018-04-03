#include "p2p.h"

#include <stdio.h>
#include <string.h> // memset()
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/epoll.h>

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
        Logger::log(LogType::P2P, ERROR, "getaddrinfo");
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
        Logger::log(LogType::P2P, ERROR, "setsockopt");
        return true;
    }

    // Bind to the address
    if (bind(mMainSocket, res->ai_addr, res->ai_addrlen) == -1) {
        Logger::log(LogType::P2P, ERROR, "bind");
        return true;
    }

    // Listen
    if (listen(mMainSocket, mListenQueue) == -1) {
        Logger::log(LogType::P2P, ERROR, "listen");
        return true;
    }
    freeaddrinfo(res);

    Logger::log(LogType::P2P, INFO, "opened listen socket on port %s", mPort);

    //Launch thread
    mThread = std::thread(&P2P::thread_loop, this);
    mRunning = true;

    return false;
}

bool P2P::stop(){
    if(mRunning){
        //Send signal to thread via signalfd
        //TODO

        mThread.join();
    }
    mRunning = false;
    mMainSocket = 0;
}

int P2P::thread_loop(void) {
    //Inaitialize epoll
    int epfd = epoll_create(1);
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = -1; //-1 is the main socket, not a client socket
    epoll_ctl(epfd, EPOLL_CTL_ADD, mMainSocket, &event);

    // Main loop
    while (mRunning) {
        if(epoll_wait(epfd, &event, 1, -1) == -1){
            Logger::log(LogType::P2P, ERROR, "epoll error %d", errno);
            break;
        }

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
                mClientSocket[newsock] = their_addr;
                event.events = EPOLLIN;
                event.data.fd = newsock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, newsock, &event);
            }
        }else{
            //Attend this socket
            char buffer[4096];
            int valread = -1;
            if ((valread = read(event.data.fd, buffer, 4096)) == 0){
                //Somebody disconnected
                const auto their_addr = mClientSocket[event.data.fd];
                Logger::log(LogType::P2P, INFO, "Disconnected (id %d) from %s on port %d",
                        event.data.fd, inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));

                //Close the socket and remove from poll
                close(event.data.fd);
                mClientSocket.erase(event.data.fd);
                epoll_ctl(epfd, EPOLL_CTL_DEL, event.data.fd, NULL);
            }else{
                //Packet on already connected client
                Logger::log(LogType::P2P, DEBUG, "Packet on connected client (size %s)", valread);

                //set the string terminating NULL byte on the end of the data read
                //buffer[valread] = '\0';
                //send(sd , buffer , strlen(buffer) , 0 );
            }
        }
    }

    //Stop epoll
    close(epfd);

    //Stop all sockets
    for(auto& i : mClientSocket){
        close(i.first);
    }
    mClientSocket.clear();
    close(mMainSocket);

    return 0;
}

} //namespace DBC
