#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

void sendfile(FILE *fp, int sockfd);
ssize_t total = 0;

int main(int argc, char* argv[]) {
    if (argc != 3) {
      perror("usage: binExec filepath IPaddress");
      exit(1);
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
      perror("Can't allocate sockfd.");
      exit(1);
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8877);

    if (inet_pton(AF_INET, argv[2], &serveraddr.sin_addr) < 0) {
      perror("IPaddress convert error.");
      exit(1);
    }

    if (connect(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
      perror("Connect error.");
      exit(1);
    }

    char *filename = basename(argv[1]);
    if (filename == NULL) {
      perror("Can't get filename.");
      exit(1);
    }

    char buff[2048] = {0};
    strncpy(buff, filename, strlen(filename));
    if (send(sockfd, buff, 2048, 0) == -1) {
      perror("Can't send filename.");
      exit(1);
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
      perror("Can't open file.");
      exit(1);
    }

    sendfile(fp, sockfd);
    printf("Sent. Bytes sent = %ld\n", total);
    fclose(fp);
    close(sockfd);
    return 0;
}

void sendfile(FILE *fp, int sockfd) {
  int n;
  char sendline[2048] = {0};
  while ((n = fread(sendline, sizeof(char), 2048, fp)) > 0) {
    total += n;
    if (n != 2048 && ferror(fp)) {
      perror("Read file error.");
      exit(1);
    }

    if (send(sockfd, sendline, n, 0) == -1) {
      perror("Can't send file");
      exit(1);
    }
    memset(sendline, 0, 2048);
  }
}
