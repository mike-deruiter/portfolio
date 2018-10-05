#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFF_LEN    1024
#define PORT        8080
#define ADDRESS     "127.0.0.1"

int main(int argc, char *argv[])
{
    struct sockaddr_in address, serv_addr;
    int sockfd = 0, valread;
    char *hello = "Greetings from client";
    char buffer[BUFF_LEN] = {0};
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("client: socket failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr)); // ???
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, ADDRESS, &serv_addr.sin_addr) <= 0)
    {
        perror("client: invalid address / not supported");
        exit(EXIT_FAILURE);
    }
    
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("client: connection failed");
        exit(EXIT_FAILURE);
    }
    
    send(sockfd, hello, strlen(hello), 0);
    printf("Message Sent:     %s\n", hello);
    valread = read(sockfd, buffer, BUFF_LEN);
    printf("Message Received: %s\n", buffer);
    
    return 0;
    
}
