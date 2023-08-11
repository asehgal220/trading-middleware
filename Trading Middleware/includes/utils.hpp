#ifndef UTILS_HPP
#define UTILS_HPP

#include <unistd.h>
#include <cerrno>

ssize_t Read(int socket, char* buffer, size_t bytesToRead) {
    ssize_t bytesRead = 0;
    while(bytesRead < bytesToRead) {
        ssize_t bytesLoaded = read(socket, buffer + bytesRead, bytesToRead - bytesRead);
        switch (bytesLoaded)
        {
        case 0:
            return 0;
        case -1:
            if (errno == EINTR) {
                continue;
            }
            return -1;
            
        default:
            bytesRead += bytesLoaded;
        }
    }
    return 0;
}

ssize_t Write(int socket, char* buffer, size_t bytesToWrite) {
    ssize_t bytesWritten = 0;
    while(bytesWritten < bytesToWrite) {
        ssize_t bytesLoaded = write(socket, buffer + bytesWritten, bytesToWrite - bytesWritten);
        switch (bytesLoaded)
        {
        case 0:
            return 0;
        case -1:
            if (errno == EINTR) {
                continue;
            }
            return -1;
            
        default:
            bytesWritten += bytesLoaded;
        }
    }
    return 0;
}


#endif