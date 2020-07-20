#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include "queue.h"

int throw_error(const char* msg) {
  perror(msg);
  exit(-1);
  return 0;
}

int main() {
  key_t key= ftok(PathName, ProjectId);
  if (key < 0) throw_error("Didn't get the key.");

  int qid = msgget(key, 0666 | IPC_CREAT);
  if (qid < 0) throw_error("Can't access the queue.");

  int types[] = {1, 1, 2, 2, 3, 3};
  int i;
  for (i = 0; i < MsgCount; i++) {
    queuedMessage msg;
    if (msgrcv(qid, &msg, sizeof(msg), types[i], MSG_NOERROR | IPC_NOWAIT) < 0)
      puts("Couldn't receive the message.");
    printf("%s received as %i\n", msg.payload, (int) msg.type);
  }

  if (msgctl(qid, IPC_RMID, NULL) < 0)
    throw_error("Can't remove the queue. Some error as occured.");

  return 0;
}
