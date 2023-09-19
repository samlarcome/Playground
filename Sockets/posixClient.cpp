#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_ADDR "localhost"
#define SERVER_PORT "49160"

using namespace std;

void handle_socket_error(string error_message) {
    cout << error_message << "\n";
    exit(1);
}

int main(int argc, char *argv[])
{
    int ai_result, connect_result, send_result;
    int connect_socket;

    struct addrinfo *ll_result, *ptr, hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    ai_result = getaddrinfo(SERVER_ADDR, SERVER_PORT, &hints, &ll_result);
    if (ai_result != 0) { handle_socket_error("Error Getting Address Information"); }

    for(ptr = ll_result; ptr != NULL; ptr = ptr->ai_next) {
        connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (connect_socket == -1) { handle_socket_error("Error Creating Connecting Socket"); }

        connect_result = connect(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (connect_result == -1) { 
            close(connect_socket);
            connect_socket = -1;
            continue;
        }

        break;
    }

    freeaddrinfo(ll_result);

    if (connect_socket == -1) {
        handle_socket_error("Cannot Connect to Server");
    }

    send_result = send(connect_socket, "Hello World From Client!\n", 24, 0);


    close(connect_socket);
}