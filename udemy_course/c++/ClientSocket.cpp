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
    int sockfd = 0;
    int bytesReceived = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    memset(recvBuff, '0', sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    // Set socket to non-blocking
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5555);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    std::cout << "Connecting to server..." << std::endl;

    // Try to connect (non-blocking)
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        if(errno == EINPROGRESS) {
            // Connection in progress, wait for it
            fd_set writefds;
            struct timeval tv;
            
            FD_ZERO(&writefds);
            FD_SET(sockfd, &writefds);
            tv.tv_sec = 5;  // 5 second timeout
            tv.tv_usec = 0;

            if(select(sockfd + 1, NULL, &writefds, NULL, &tv) > 0) {
                int error = 0;
                socklen_t len = sizeof(error);
                if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0) {
                    std::cout << "Connection failed!" << std::endl;
                    close(sockfd);
                    return 1;
                }
                std::cout << "Connected to server!" << std::endl;
            } else {
                std::cout << "Connection timeout" << std::endl;
                close(sockfd);
                return 1;
            }
        } else {
            std::cout << "Connection failed!" << std::endl;
            close(sockfd);
            return 1;
        }
    } else {
        std::cout << "Connected to server!" << std::endl;
    }

    // Save original terminal settings
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
    newt.c_cc[VMIN] = 1;        // Minimum number of characters for read
    newt.c_cc[VTIME] = 0;       // Time to wait for data
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::cout << "\nClient ready. Type messages and press Enter to send (type 'quit' to exit)" << std::endl;
    std::cout << "> " << std::flush;

    std::string currentInput;
    std::queue<std::string> messageQueue;
    bool running = true;

    while(running) {
        // Check for incoming messages
        memset(recvBuff, 0, sizeof(recvBuff));
        bytesReceived = recv(sockfd, recvBuff, sizeof(recvBuff), 0);
        
        if(bytesReceived > 0) {
            std::cout << "\nServer says: " << recvBuff << std::endl;
            std::cout << "> " << currentInput << std::flush;
        } else if(bytesReceived == -1) {
            if(errno != EAGAIN && errno != EWOULDBLOCK) {
                std::cout << "Error receiving data" << std::endl;
                break;
            }
        } else if(bytesReceived == 0) {
            std::cout << "Server disconnected" << std::endl;
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
            int bytesSent = send(sockfd, message.c_str(), message.length(), 0);
            
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

    close(sockfd);
    std::cout << "Disconnected from server" << std::endl;

    return 0;
}


