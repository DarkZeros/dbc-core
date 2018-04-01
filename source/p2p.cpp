#include "p2p.h"

#include <stdio.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

namespace DBC {

P2P::P2P(){
}

bool P2P::start(){
    //We first do the initialization of socket in the main thread
    //This make seasy to report errors
    //If everything in the setup succeeds, then start the thread loop

    int sock;
    struct addrinfo hints, *res;
    int reuseaddr = 1;

    // Get the address info
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(NULL, mPort, &hints, &res) != 0) {
        perror("getaddrinfo");
        return true;
    }

    // Create the socket
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        perror("socket");
        return true;
    }

    // Enable the socket to reuse the address
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("setsockopt");
        return true;
    }

    // Bind to the address
    if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        return true;
    }

    // Listen
    if (listen(sock, mListenQueue) == -1) {
        perror("listen");
        return true;
    }

    freeaddrinfo(res);

    return false;
}

void handle(int newsock)
{
    /* recv(), send(), close() */
}

int P2P::thread_loop(void)
{


    // Main loop
    /*while (1) {
        socklen_t size = sizeof(struct sockaddr_in);
        struct sockaddr_in their_addr;
        int newsock = accept(sock, (struct sockaddr*)&their_addr, &size);

        if (newsock == -1) {
            perror("accept");
        }
        else {
            printf("Got a connection from %s on port %d\n",
                    inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));
            handle(newsock);
        }
    }

    close(sock);*/

    return 0;
}

} //namespace DBC
