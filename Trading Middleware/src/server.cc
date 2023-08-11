
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

void StartServer(const char* port);
void* ProcessClient(void* clientId);

#define MAX_CLIENTS 32

int serverSocket;

void cleanup() {
    if (shutdown(serverSocket, SHUT_RDWR) == -1) {
        perror("Unable to shutdown server");
        exit(1);
    };
    close(serverSocket);
}

int main(int argc, char const* argv[]) {

    StartServer(argv[1]);

	std::cout<<"Hello CMake!"<<std::endl;
}

void StartServer(const char* port) {
    
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Unable to initialize server socket");
        exit(1);
    }

    int status;
    struct addrinfo hints;
    struct addrinfo *serverInfo;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &serverInfo) == -1) {
        freeaddrinfo(serverInfo);
        perror("Unable to load ports");
        exit(1);
    }

    if (bind(serverSocket, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
        freeaddrinfo(serverInfo);
        perror("Faliure to bind");
        exit(1);
    }

    freeaddrinfo(serverInfo);

    int optval = 1;

    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        perror("setsockopt()");
        exit(1);
    }

    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) == -1) {
        perror("setsockopt()");
        exit(1);
    }

    if(listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Failed to listen on port");
        exit(1);
    }

    while(true) {
        int connectionSocket = accept(serverSocket, NULL, NULL);
        if (connectionSocket == -1) {
            perror("accept()");
            exit(1);
        }
        pthread_t threadId;
        pthread_create(&threadId, NULL, ProcessClient, (void*)(long)connectionSocket);
        pthread_detach(threadId);
    }
}

void* ProcessClient(void* clientId) {
    int port = (int)(long)clientId;
    char* buffer = new char[5];
    Read(port, buffer, 5);
    std::cout << buffer << std::endl;
    delete[] buffer;
    pthread_exit(NULL);

}