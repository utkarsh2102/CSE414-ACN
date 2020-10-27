#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define MAXLINE 100
#define SERV_PORT 13153

int main (int argc, char **argv) {
  int k, i, maxi, maxfd, listenfd, connfd, sockfd;
  int nready, client[FD_SETSIZE];
  ssize_t n;
  fd_set rset, allset;
  char line[MAXLINE], buf[100];
  socklen_t clilen;
  struct sockaddr_in cliaddr, servaddr;
  listenfd = socket (AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) {
    perror ("Socket error.");
    exit (1);
  }
  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
  servaddr.sin_port = htons (SERV_PORT);
  bind (listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
  listen (listenfd, 5);
  maxfd = listenfd;
  maxi = -1;
  for (i = 0; i < FD_SETSIZE; i++)
    client[i] = -1;
  FD_ZERO (&allset);
  FD_SET (listenfd, &allset);

  for (;;) {
    printf ("Server started, at main loop.\n");
    rset = allset;
    nready = select (maxfd + 1, &rset, NULL, NULL, NULL);
    if (FD_ISSET (listenfd, &rset))
  	{
	    clilen = sizeof (cliaddr);
	    connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
#ifdef NOTDEF
	    printf ("New client: %s, port %d.\n",
		  inet_ntop (AF_INET, &cliaddr.sin_addr, buf, NULL),
		  ntohs (cliaddr.sin_port));
#endif
	  for (i = 0; i < FD_SETSIZE; i++)
	    if (client[i] < 0) {
  	  	client[i] = connfd;
	  	  break;
	    }
	    if (i == FD_SETSIZE) {
	      printf ("Utkarsh, that's too many clients.");
	      exit (0);
	    }
	    FD_SET (connfd, &allset);
	    if (connfd > maxfd)
	      maxfd = connfd;
	    if (i > maxi)
	      maxi = i;
	    if (--nready <= 0) {
	      for (i = 0; i <= maxi; i++) {
      		if ((sockfd = client[i]) < 0)
		        continue;
		      if (FD_ISSET (sockfd, &rset)) {
	    	    if ((n = read (sockfd, line, MAXLINE)) == 0) {
			        close (sockfd);
			        FD_CLR (sockfd, &allset);
			        client[i] = -1;
		        }
    		    else {
        			printf ("\nOutput at server.\n");
			        for (k = 0; line[k] != '\0'; k++)
			          printf ("%c", toupper (line[k]));
			        write (sockfd, line, n);
		        }
		        if (--nready <= 0)
		         break;
  		    }
	  	  }
	    }
  	}
  }
}
