/*
 * Server Socket Program in C
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

int main()
{
    // Socket file descriptors
    int server = 0;    // File descriptor for server socket
    int client = 0;    // File descriptor for client socket
    
    // Structure to hold server address information
    struct sockaddr_in serverAddr;
    
    // Message to send to client
    char * message = "Hello dear \r\n";

    // Create a TCP socket
    // AF_INET: IPv4 protocol
    // SOCK_STREAM: TCP socket type
    // 0: Protocol (0 means default protocol for the given type)
    server = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address structure
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    serverAddr.sin_family = AF_INET;          // IPv4 address family
    serverAddr.sin_port = htons(5555);        // Port number (converted to network byte order)

    // Bind the socket to the specified address and port
    // This associates the socket with a specific network interface and port
    bind(server, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Start listening for incoming connections
    // 0: Maximum length of the queue of pending connections
    listen(server, 0);

    printf("Listening for incoming connections...\n");

    // Accept an incoming connection
    // This blocks until a client connects
    // NULL parameters mean we don't need client address information
    client = accept(server, (struct sockaddr*)NULL, NULL);
    printf("Client connected! \n");

    printf("before calling send in server \n");
    // Send message to client
    // Parameters:
    // - client: Client socket file descriptor
    // - message: Data to send
    // - strlen(message): Length of data
    // - 0: Flags (0 means no special flags)
    if(send(client, message, strlen(message),0) != strlen(message))
    {
        printf("in if of send in server \n");
        printf("Error in sending to client \n");
    }
    else
    {
        printf("In else condition of send in server \n");
    }
    printf("after send \n");

    // Close the client connection
    close(client);
    printf("Client disconnected \n");
}


