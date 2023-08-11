
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "utils.hpp"
#include "client_utils.hpp"

int ConnectToServer(const char *host, const char *port);

int main(int argc, char* argv[]) {
    std::cout << "Hello" << std::endl;
    char** connectionInfo = ParseArgs(argc, argv);
    std::cout << connectionInfo[0] << std::endl;
    std::cout << connectionInfo[1] << std::endl;
    int serverSocket = ConnectToServer(connectionInfo[0], connectionInfo[1]);
    char* tmp = "HEllo";
    Write(serverSocket, tmp, 5);
    shutdown(serverSocket, SHUT_WR);
}

int ConnectToServer(const char *host, const char *port) {
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1) {
        perror(NULL);
        exit(1);
    }
    struct addrinfo hints;
    struct addrinfo *result = NULL;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo(host, port, &hints, &result);
    if (err != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        exit(1);
    }

    err = connect(socketFd, result->ai_addr, result->ai_addrlen);
    if (err == -1) {
        perror(NULL);
        exit(1);
    }
    freeaddrinfo(result);
    return socketFd;
}