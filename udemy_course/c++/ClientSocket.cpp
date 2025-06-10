/*
 * Client Socket Program in C++
 * This program creates a TCP client that connects to a server
 * and receives a message from it.
 */

#include <sys/socket.h>    // For socket functions
#include <sys/types.h>     // For system types
#include <netinet/in.h>    // For sockaddr_in structure
#include <netdb.h>         // For network database operations
#include <stdio.h>         // For printf
#include <string.h>        // For string operations
#include <stdlib.h>        // For exit
#include <unistd.h>        // For close
#include <errno.h>         // For error handling
#include <arpa/inet.h>     // For inet_addr function
#include <iostream>        // For C++ I/O operations

int main()
{
    // Socket file descriptor for the client
    int serverSock = 0;
    
    // Structure to hold server address information
    struct sockaddr_in addr;

    // Create a TCP socket
    // AF_INET: IPv4 protocol
    // SOCK_STREAM: TCP socket type
    // 0: Protocol (0 means default protocol for the given type)
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address structure
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP (localhost)
    addr.sin_family = AF_INET;                      // IPv4 address family
    addr.sin_port = htons(5555);                    // Server port (converted to network byte order)

    // Connect to the server
    // This establishes a connection with the server
    connect(serverSock, (struct sockaddr *)&addr, sizeof(addr));
    std::cout << "Connected to server!" << std::endl;

    // Buffer to store received data
    char buffer[1025];
    int val;

    // Read data from the server
    // Parameters:
    // - serverSock: Socket file descriptor
    // - buffer: Buffer to store received data
    // - 1024: Maximum number of bytes to read
    if((val = read(serverSock, buffer, 1024)) != 0)
    {
        // Null terminate the received data
        buffer[val] = '\0';
        std::cout << "Received message from server: " << buffer << std::endl;
    }

    // Close the socket connection
    close(serverSock);
    std::cout << "Socket closed." << std::endl;

    return 0;
}


