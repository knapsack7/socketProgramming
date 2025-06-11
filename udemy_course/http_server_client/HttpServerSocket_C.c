#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>


int main()
{
    int server = 0, client = 0;//Linux specifix
    struct sockaddr_in serverAddr;//linux specific

    char message[9999];
    strcpy(message,"HTTP/1.1 200 OK\nContent-length: 47\nContent-Type: text/html\n\n<html><body><H1>Learning Socket Programming</H1></body></html>");

    server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);

    bind(server, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(server, 0);

    
    printf("Listening for incoming connections...\n");

    char buffer[1024] = {0};
    int msg_len;
  
	
    client = accept(server, (struct sockaddr*)NULL, NULL);
    printf("Client connected! \n");
    read(client, buffer, sizeof(buffer)-1);

    printf("client says: buffer is %s \n", buffer);
        	
    memset(buffer, 0, sizeof(buffer));


    msg_len = write(client, message, strlen(message));
    if (msg_len == 0)
    {
        printf("Client closed connection\n");
        close(client);
        return -1;
    }

    close(client);
    printf("Client disconnected \n");
	
}


