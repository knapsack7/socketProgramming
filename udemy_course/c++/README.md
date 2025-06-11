# TCP Chat Application (V2) - Non-blocking Socket Communication

This project demonstrates a TCP chat application with non-blocking socket communication between a server and client. The application evolved from a basic blocking implementation (V1) to a more robust non-blocking version (V2) that handles multiple operations concurrently.

## Evolution from V1 to V2

### V1: Basic Blocking Implementation
The initial version used blocking sockets, which meant:
- Server could only handle one client at a time
- Input/output operations would block the entire application
- No proper handling of terminal input (backspace, control characters)

### V2: Non-blocking Implementation
The current version implements non-blocking sockets with proper terminal handling:
- Server can handle multiple clients (though currently limited to one for simplicity)
- Non-blocking I/O operations
- Proper terminal input handling
- Message queuing system

## Key Features

1. **Non-blocking Socket Operations**
   ```cpp
   // Set socket to non-blocking mode
   int flags = fcntl(sockfd, F_GETFL, 0);
   fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
   ```

2. **Proper Terminal Settings**
   ```cpp
   // Save original terminal settings
   struct termios oldt, newt;
   tcgetattr(STDIN_FILENO, &oldt);
   newt = oldt;
   newt.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
   newt.c_cc[VMIN] = 1;        // Minimum number of characters for read
   newt.c_cc[VTIME] = 0;       // Time to wait for data
   tcsetattr(STDIN_FILENO, TCSANOW, &newt);
   ```

3. **Character-by-Character Input Handling**
   ```cpp
   if(read(STDIN_FILENO, &c, 1) > 0) {
       if(c == '\r' || c == '\n') {
           // Handle Enter key
       } else if(c == 127 || c == 8) {  // Backspace or Delete
           if(!currentInput.empty()) {
               currentInput.pop_back();
               std::cout << "\b \b" << std::flush;
           }
       } else if(c >= 32 && c <= 126) {  // Printable ASCII
           currentInput += c;
           std::cout << c << std::flush;
       }
   }
   ```

4. **Message Queuing System**
   ```cpp
   std::queue<std::string> messageQueue;
   // ... in the main loop ...
   while(!messageQueue.empty()) {
       std::string& message = messageQueue.front();
       int bytesSent = send(sockfd, message.c_str(), message.length(), 0);
       if(bytesSent > 0) {
           messageQueue.pop();
       }
   }
   ```

## Issues Encountered and Solutions

### 1. Blocking Socket Operations
**Issue**: Initial implementation used blocking sockets, causing the application to freeze during I/O operations.

**Solution**: Implemented non-blocking sockets using `fcntl()` and proper error handling for `EAGAIN`/`EWOULDBLOCK`.

### 2. Terminal Input Handling
**Issue**: Backspace and control characters were being displayed as `^?` instead of being handled properly.

**Solution**: 
- Disabled both canonical mode and echo
- Implemented manual character-by-character input handling
- Added proper backspace handling with cursor movement

### 3. Connection Timeout
**Issue**: Client would hang indefinitely when trying to connect to a non-existent server.

**Solution**: Implemented connection timeout using `select()`:
```cpp
if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    if(errno == EINPROGRESS) {
        fd_set writefds;
        struct timeval tv;
        FD_ZERO(&writefds);
        FD_SET(sockfd, &writefds);
        tv.tv_sec = 5;  // 5 second timeout
        if(select(sockfd + 1, NULL, &writefds, NULL, &tv) > 0) {
            // Check connection status
        }
    }
}
```

### 4. Message Synchronization
**Issue**: Messages could be lost or corrupted due to non-blocking send operations.

**Solution**: Implemented a message queue system to ensure reliable message delivery.

## Building and Running

1. Compile the server:
```bash
g++ -o server ServerSocket.cpp
```

2. Compile the client:
```bash
g++ -o client ClientSocket.cpp
```

3. Run the server:
```bash
./server
```

4. Run the client:
```bash
./client
```

## Usage

1. Start the server first
2. Start the client
3. Type messages and press Enter to send
4. Type 'quit' to exit

## Future Improvements

1. Support for multiple clients
2. Message history
3. User authentication
4. Encrypted communication
5. GUI interface

## Dependencies

- C++11 or later
- POSIX-compliant operating system
- Standard C++ libraries
- POSIX socket libraries 