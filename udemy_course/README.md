# Socket Programming Course - A to Z (Windows & Linux in C/C++)

## Course Overview
This repository contains the practical implementations from the Udemy course "Socket Programming A to Z - Windows & Linux in C/C++". The course covers fundamental concepts of network programming and socket implementation across different operating systems.

## Course Link
[Socket Programming A to Z - Windows & Linux in C/C++](https://www.udemy.com/course/socket-programming-a-to-z-windows-linux-in-c-c/learn/lecture/18100571#questions)

## What I've Learned

### 1. Socket Programming Fundamentals
- Understanding of TCP/IP protocol stack
- Socket types and their characteristics
- Client-Server architecture
- Network byte order and host byte order
- Socket address structures

### 2. Implementation in C
- Basic TCP server and client implementation
- Socket creation and configuration
- Binding sockets to addresses and ports
- Listening for incoming connections
- Accepting client connections
- Sending and receiving data
- Proper socket cleanup and resource management

### 3. Implementation in C++
- Object-oriented approach to socket programming
- C++ specific socket implementations
- Using C++ streams for I/O operations
- Error handling in C++ style

### 4. Key Concepts Covered
- Socket file descriptors
- Blocking vs Non-blocking operations
- Connection handling
- Data transmission
- Error handling
- Port binding
- Localhost communication

### 5. Practical Skills
- Writing server applications
- Writing client applications
- Debugging network programs
- Understanding socket states
- Handling connection errors
- Managing socket resources

## Project Structure
```
udemy_course/
├── c/                  # C implementation
│   ├── ServerSocket_C.c
│   └── ClientSocket_C.c
├── c++/               # C++ implementation
│   ├── ServerSocket.cpp
│   └── ClientSocket.cpp
└── README.md
```

## How to Run
1. Compile the server:
   ```bash
   # For C
   gcc -o server ServerSocket_C.c
   # For C++
   g++ -o server ServerSocket.cpp
   ```

2. Compile the client:
   ```bash
   # For C
   gcc -o client ClientSocket_C.c
   # For C++
   g++ -o client ClientSocket.cpp
   ```

3. Run the programs:
   ```bash
   # Terminal 1 - Run server
   ./server
   
   # Terminal 2 - Run client
   ./client
   ```

## Key Takeaways
1. Understanding of network programming fundamentals
2. Ability to implement basic client-server applications
3. Knowledge of socket programming in both C and C++
4. Understanding of TCP/IP communication
5. Experience with cross-platform socket programming

## Future Learning Path
- Multi-threaded server implementation
- Non-blocking I/O
- UDP socket programming
- Network security
- Advanced error handling
- Socket programming with other protocols

## Note
This course provides a solid foundation in socket programming. The implementations are basic but demonstrate the core concepts of network programming. For production applications, additional features like error handling, security, and scalability would need to be implemented. 