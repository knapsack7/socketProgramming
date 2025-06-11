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
#include <string>

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
    if(connect(serverSock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cout << "Connection failed!" << std::endl;
        return -1;
    }
    std::cout << "Connected to server!" << std::endl;

    char buffer[1024];
    std::string message;

    while(true) {
        // Get message from user
        std::cout << "Enter your message (or 'quit' to exit): ";
        std::getline(std::cin, message);

        // Send message to server
        if(send(serverSock, message.c_str(), message.length(), 0) == -1) {
            std::cout << "Error sending message" << std::endl;
            break;
        }

        // Check if user wants to quit
        if(message == "quit") {
            std::cout << "Client is shutting down" << std::endl;
            break;
        }

        // Clear buffer
        memset(buffer, 0, sizeof(buffer));
        
        // Receive response from server
        int bytesReceived = recv(serverSock, buffer, sizeof(buffer), 0);
        if(bytesReceived <= 0) {
            std::cout << "Server disconnected or error occurred" << std::endl;
            break;
        }
        
        std::cout << "Server says: " << buffer << std::endl;

        // Check if server wants to quit
        if(strcmp(buffer, "quit") == 0) {
            std::cout << "Server requested to quit" << std::endl;
            break;
        }
    }

    // Close the socket connection
    close(serverSock);
    std::cout << "Connection closed." << std::endl;

    return 0;
}


