# Simple HTTP Server Implementation

This project demonstrates a basic HTTP server implementation in C using socket programming. It serves as a learning exercise for understanding network programming concepts and HTTP protocol basics.

## Key Concepts Learned

### 1. Socket Programming Basics
- Creating a socket using `socket()` function
- Understanding socket types (SOCK_STREAM for TCP)
- Socket address structures (struct sockaddr_in)
- Network byte order conversion (htons)

### 2. Server Setup Process
1. Socket Creation
   ```c
   server = socket(AF_INET, SOCK_STREAM, 0);
   ```
2. Address Configuration
   ```c
   serverAddr.sin_addr.s_addr = INADDR_ANY;
   serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(8080);
   ```
3. Binding
   ```c
   bind(server, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
   ```
4. Listening
   ```c
   listen(server, 0);
   ```

### 3. HTTP Protocol Basics
- HTTP Response Structure
  - Status line (HTTP/1.1 200 OK)
  - Headers (Content-length, Content-Type)
  - Body (HTML content)
- Basic HTTP Headers
  - Content-length
  - Content-Type

### 4. Client-Server Communication
- Accepting client connections
- Reading client requests
- Sending HTTP responses
- Connection handling and cleanup

## How to Use

1. Compile the server:
   ```bash
   gcc HttpServerSocket_C.c -o server
   ```

2. Run the server:
   ```bash
   ./server
   ```

3. Access the server:
   - Open a web browser
   - Navigate to `http://localhost:8080`
   - Or use curl: `curl http://localhost:8080`

## Features
- Listens on port 8080
- Responds with a simple HTML page
- Handles basic HTTP requests
- Demonstrates proper socket cleanup

## Learning Outcomes
1. Understanding of socket programming fundamentals
2. Basic HTTP protocol implementation
3. Client-server communication patterns
4. Network programming in C
5. Proper resource management and cleanup

## Next Steps
- Implement request parsing
- Add support for different HTTP methods
- Handle multiple clients
- Add error handling
- Implement proper HTTP response generation 