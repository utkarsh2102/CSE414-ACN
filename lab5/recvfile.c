#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

void writefile(int sockfd, FILE *fp);
ssize_t total = 0;

int main(int argc, char *argv[]) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("Can't allocate sockfd.");
    exit(1);
  }

  struct sockaddr_in clientaddr, serveraddr;
  memset(&serveraddr, 0, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(8877);

  if (bind(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) == -1) {
    perror("Bind error.");
    exit(1);
  }

  if (listen(sockfd, 7788) == -1) {
    perror("Listen error.");
    exit(1);
  }

  socklen_t addrlen = sizeof(clientaddr);
  int connfd = accept(sockfd, (struct sockaddr *) &clientaddr, &addrlen);
  if (connfd == -1) {
    perror("Connect error.");
    exit(1);
  }
  close(sockfd);

  char filename[2048] = {0};
  if (recv(connfd, filename, 2048, 0) == -1) {
    perror("Can't receive filename.");
    exit(1);
  }

  FILE *fp = fopen(filename, "wb");
  if (fp == NULL) {
    perror("Can't open file.");
    exit(1);
  }

  char addr[INET_ADDRSTRLEN];
  printf("Start receive file: %s from %s\n", filename, inet_ntop(AF_INET, &clientaddr.sin_addr, addr, INET_ADDRSTRLEN));
  writefile(connfd, fp);
  printf("Received. Bytes receieved = %ld\n", total);

  fclose(fp);
  close(connfd);
  return 0;
}

void writefile(int sockfd, FILE *fp) {
  ssize_t n;
  char buff[2048] = {0};
  while ((n = recv(sockfd, buff, 2048, 0)) > 0) {
   total += n;
   if (n == -1) {
     perror("Receive file error.");
     exit(1);
   }

   if (fwrite(buff, sizeof(char), n, fp) != n) {
     perror("Write file error.");
     exit(1);
   }

   memset(buff, 0, 2048);
  }
}
