#ifndef CLIENT_UTILS_HPP
#define CLIENT_UTILS_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** ParseArgs(int argc, char* argv[]) {
    if (argc < 2) {
        return NULL;
    }

    char* host = strtok(argv[1], ":");
    char* port = strtok(NULL, ":");
    if (port == NULL) {
        return NULL;
    }

    char** args = new char*[3];
    args[0] = host;
    args[1] = port;
    args[2] = NULL;

    return args;
}

#endif