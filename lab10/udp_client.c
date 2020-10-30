#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define PORT 5000
#define MAXLINE 1024

int main()
{
    int sockfd;
    char buffer[MAXLINE];
    char* message = "Hello server.";
    struct sockaddr_in servaddr;

    int n, len;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket creation failed");
        exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sendto(sockfd, (const char*)message, strlen(message),
           0, (const struct sockaddr*)&servaddr,
           sizeof(servaddr));

    printf("Message from server: ");
    n = recvfrom(sockfd, (char*)buffer, MAXLINE,
                 0, (struct sockaddr*)&servaddr,
                 &len);
    puts(buffer);
    close(sockfd);
    return 0;
}
