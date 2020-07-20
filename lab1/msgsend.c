#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

int throw_error(const char* msg) {
  perror(msg);
  exit(-1);
  return 0;
}

int main() {
  key_t key = ftok(PathName, ProjectId);
  if (key < 0) {
    throw_error("Couldn't get the key.");
  }

  int qid = msgget(key, 0666 | IPC_CREAT);
  if (qid < 0) {
    throw_error("Couldn't get queue ID.");
  }

  char* payloads[] = {"MSG1: TEST", "MSG2: hope", "MSG3: that", "MSG4: it", "MSG5: is", "MSG6: WORKING!"};
  int types[] = {1, 1, 2, 2, 3, 3};
  int i;
  for (i = 0; i < MsgCount; i++) {

    queuedMessage msg;
    msg.type = types[i];
    strcpy(msg.payload, payloads[i]);

    msgsnd(qid, &msg, sizeof(msg), IPC_NOWAIT);
    printf("%s sent as %i\n", msg.payload, (int) msg.type);
  }

  return 0;
}
