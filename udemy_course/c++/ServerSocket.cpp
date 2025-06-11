/*
 * Server Socket Program in C++
 * This program creates a TCP server that listens for incoming connections
 * and sends a message to any connected client.
 */

#include <sys/socket.h>    // For socket functions
#include <netinet/in.h>    // For sockaddr_in structure
#include <arpa/inet.h>     // For inet_ntoa function
#include <stdio.h>         // For printf
#include <stdlib.h>        // For exit
#include <unistd.h>        // For close
#include <errno.h>         // For error handling
#include <string.h>        // For string operations
#include <sys/types.h>     // For system types
#include <iostream>        // For C++ I/O operations
#include <string>

int main()
{
    // Socket file descriptors
    int server = 0;    // File descriptor for server socket
    int client = 0;    // File descriptor for client socket
    
    // Structure to hold server address information
    struct sockaddr_in serverAddr;
    
    // Create a TCP socket
    // AF_INET: IPv4 protocol
    // SOCK_STREAM: TCP socket type
    // 0: Protocol (0 means default protocol for the given type)
    // socket for listening incoming connections
    server = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address structure
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    serverAddr.sin_family = AF_INET;          // IPv4 address family
    serverAddr.sin_port = htons(5555);        // Port number (converted to network byte order)

    // Bind the socket to the specified address and port
    // This associates the socket with a specific network interface and port
    bind(server, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Start listening for incoming connections
    // 5: Maximum length of the queue of pending connections,
    // if there would be more than 5 pending connections, the server will reject the 
    // new connections
    listen(server, 5);

    std::cout << "Server is listening for connections..." << std::endl;

    // Accept an incoming connection
    // This blocks until a client connects
    // NULL parameters mean we don't need client address information
    client = accept(server, (struct sockaddr*)NULL, NULL);
    std::cout << "Client connected!" << std::endl;

    char buffer[1024];
    std::string message;

    while(true) {
        // Clear buffer
        memset(buffer, 0, sizeof(buffer));
        
        // Receive message from client
        int bytesReceived = recv(client, buffer, sizeof(buffer), 0);
        if(bytesReceived <= 0) {
            std::cout << "Client disconnected or error occurred" << std::endl;
            break;
        }
        
        std::cout << "Client says: " << buffer << std::endl;

        // Check if client wants to quit
        if(strcmp(buffer, "quit") == 0) {
            std::cout << "Client requested to quit" << std::endl;
            break;
        }

        // Get server's response
        std::cout << "Enter your message (or 'quit' to exit): ";
        std::getline(std::cin, message);

        // Send response to client
        if(send(client, message.c_str(), message.length(), 0) == -1) {
            std::cout << "Error sending message" << std::endl;
            break;
        }

        // Check if server wants to quit
        if(message == "quit") {
            std::cout << "Server is shutting down" << std::endl;
            break;
        }
    }

    // Close the client connection
    close(client);
    std::cout << "Client disconnected" << std::endl;
    close(server);
    std::cout << "Server closed" << std::endl;

    return 0;
}


