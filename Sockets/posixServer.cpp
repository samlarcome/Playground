#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 49160 

int main() {

    /*
        struct sockaddr_in (netinet/in.h): This is the structure used to represent an IPv4 socket address
            - sin_family (the address family, typically set to AF_INET for IPv4)
            - sin_port (the port number)
            - sin_addr (the IP address)
    */
    struct sockaddr_in serv_addr, cli_addr;
    int sockFD, newsockFD, n, clilen;
    char buffer[256];

    
    sockFD = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFD == -1) {
        std::cout << "Error creating socket FD";
        return 1;
    }

    // clear address structure
    memset(&serv_addr, 0, sizeof(serv_addr));

    // Set serv_addr to user IPv4 addresses
    serv_addr.sin_family = AF_INET;

    // socket will accept connections on all available network interfaces and IP addresses of the server.
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // converted from host byte order to network byte order using htons()
    // necessary because network protocols, including TCP and UDP, use network byte order (big-endian) for port numbers
    serv_addr.sin_port = htons(PORT);

    //
    if (bind(sockFD, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Error binding socket";
        return 1;
    }

    listen(sockFD,5);

    clilen = sizeof(cli_addr);

    while (true) {
        socklen_t clilen = sizeof(cli_addr);
        newsockFD = accept(sockFD, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockFD == -1) {
            std::cerr << "Error accepting connection";
            return 1;
        }

        // Handle the new connection here, possibly in a separate thread or process.
        printf("server: got connection from %s port %d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

        // This send() function sends the 13 bytes of the string to the new socket
        send(newsockFD, "Hello, world!\n", 13, 0);

        bzero(buffer,256);

        n = read(newsockFD,buffer,255);
        if (n < 0) {
            std::cout << "ERROR reading from socket"; 
        }

        printf("Here is the message: %s\n",buffer);

        close(newsockFD);
        close(sockFD);
        return 0; 
    }

}