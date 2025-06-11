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
#include <fcntl.h>  // For fcntl
#include <queue>
#include <termios.h>

// Function to check if there's input available on stdin
bool isInputAvailable() {
    fd_set readfds;
    struct timeval tv;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0;
}

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

    // Set server socket to non-blocking
    int flags = fcntl(server, F_GETFL, 0);
    fcntl(server, F_SETFL, flags | O_NONBLOCK);

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

    // Wait for client connection with timeout
    fd_set readfds;
    struct timeval tv;
    bool clientConnected = false;

    while(!clientConnected) {
        FD_ZERO(&readfds);
        FD_SET(server, &readfds);
        tv.tv_sec = 5;  // 5 second timeout
        tv.tv_usec = 0;

        int ready = select(server + 1, &readfds, NULL, NULL, &tv);
        
        if(ready > 0) {
            client = accept(server, (struct sockaddr*)NULL, NULL);
            if(client > 0) {
                // Set client socket to non-blocking
                flags = fcntl(client, F_GETFL, 0);
                fcntl(client, F_SETFL, flags | O_NONBLOCK);
                std::cout << "Client connected!" << std::endl;
                clientConnected = true;
            }
        } else if(ready == 0) {
            std::cout << "Waiting for client connection..." << std::endl;
        } else {
            std::cout << "Error in select" << std::endl;
            break;
        }
    }

    if(!clientConnected) {
        std::cout << "No client connected, shutting down server" << std::endl;
        close(server);
        return -1;
    }

    char buffer[1024];
    std::queue<std::string> messageQueue;
    bool running = true;

    // Save original terminal settings
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
    newt.c_cc[VMIN] = 1;        // Minimum number of characters for read
    newt.c_cc[VTIME] = 0;       // Time to wait for data
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::cout << "\nServer ready. Type messages and press Enter to send (type 'quit' to exit)" << std::endl;
    std::cout << "> " << std::flush;

    std::string currentInput;
    while(running) {
        // Check for incoming messages
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(client, buffer, sizeof(buffer), 0);
        
        if(bytesReceived > 0) {
            std::cout << "\nClient says: " << buffer << std::endl;
            std::cout << "> " << currentInput << std::flush;
        } else if(bytesReceived == -1) {
            if(errno != EAGAIN && errno != EWOULDBLOCK) {
                std::cout << "Error receiving data" << std::endl;
                break;
            }
        } else if(bytesReceived == 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        // Check for user input
        if(isInputAvailable()) {
            char c;
            if(read(STDIN_FILENO, &c, 1) > 0) {
                if(c == '\r' || c == '\n') {
                    if(!currentInput.empty()) {
                        if(currentInput == "quit") {
                            running = false;
                        } else {
                            messageQueue.push(currentInput);
                        }
                        currentInput.clear();
                        std::cout << "\n> " << std::flush;
                    }
                } else if(c == 127 || c == 8) {  // Backspace or Delete
                    if(!currentInput.empty()) {
                        currentInput.pop_back();
                        std::cout << "\b \b" << std::flush;
                    }
                } else if(c >= 32 && c <= 126) {  // Only accept printable ASCII characters
                    currentInput += c;
                    std::cout << c << std::flush;
                }
            }
        }

        // Send queued messages
        while(!messageQueue.empty()) {
            std::string& message = messageQueue.front();
            int bytesSent = send(client, message.c_str(), message.length(), 0);
            
            if(bytesSent == -1) {
                if(errno == EAGAIN || errno == EWOULDBLOCK) {
                    // Buffer full, try again later
                    break;
                } else {
                    std::cout << "Error sending message" << std::endl;
                    running = false;
                    break;
                }
            } else {
                messageQueue.pop();
            }
        }

        // Small delay to prevent CPU spinning
        usleep(10000);  // 10ms delay
    }

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    // Close the client connection
    close(client);
    std::cout << "Client disconnected" << std::endl;
    close(server);
    std::cout << "Server closed" << std::endl;

    return 0;
}


