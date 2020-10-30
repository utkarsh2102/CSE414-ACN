#include "rpctime.h"

void rpctime_1(char *host) {
  CLIENT *clnt;
  long *result_1;
  char *gettime_1_arg;

  #ifndef DEBUG
  clnt = clnt_create (host, RPCTIME, RPCTIMEVERSION, "udp");
  if (clnt == NULL) {
    clnt_pcreateerror (host);
    exit (1);
  }
  #endif/* DEBUG */

  result_1 = gettime_1((void*)&gettime_1_arg, clnt);
  if (result_1 == (long *) NULL) {
    clnt_perror (clnt, "call failed");
  }
  else
    printf("%d |%s", *result_1, ctime(result_1));

  #ifndef DEBUG
  clnt_destroy (clnt);
  #endif /* DEBUG */
}

int main (int argc, char *argv[]) {
  char *host;
  if (argc < 2) {
    printf ("usage: %s server_host\n", argv[0]);
    exit (1);
  }
  host = argv[1];
  rpctime_1 (host);
  exit (0);
}
