#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
// #include <sys/wait.h>
#include <unistd.h>

int main() {
  int i;               // loop variable
  key_t shmkey;        // need a key to access shared memory
  int shmid;           // shared memory ID
  sem_t *sem;          // semaphore (defined in <sys/types.h>)
  pid_t pid;           // PID and fork() (defined in <sys/types.h>)
  int *p;              // shared variable
  unsigned int n;      // fork() count (from the user)
  unsigned int value;  // semaphore value (from the user)

  // Part 1: Initialization of the shared memory.
  shmkey = ftok("/home/utkarsh/labs/semapohore.c", 5);
  printf("shmkey for p = %d\n", shmkey);
  shmid = shmget(shmkey, sizeof(int), 0644|IPC_CREAT);

  if(shmid < 0) {
    perror("shmget\n");
    exit(1);
  }

  p = (int*)shmat(shmid, NULL, 0);
  *p = 0;
  printf("p = %d is allocated in the shared memory.\n\n", *p);

  // Part 2: Forking and semaphore declaration.
  printf("Enter the number of children you want to fork: ");
  scanf("%u",&n);
  printf("Enter the semaphore value: ");
  scanf("%u",&value);

  // Part 3: Semaphore initialization.
  sem = sem_open("utkarsh", O_CREAT|O_EXCL, 0644, value);
  sem_unlink("utkarsh");

  // Part 4: FORK!
  for(i=0; i < n; i++) {
    pid = fork();
    if(pid < 0)
      printf("fork error.\n");
    else if(pid == 0)  // child process.
      break;
  }

  if(pid != 0) {  // return value >0 represents a parent process.
    // wait for all the children to exit as problems of orphans
    // may arise later.
    while(pid = waitpid(-1, NULL)) {
      if(errno == ECHILD)
        break;
    }
    printf("\nAll children have exited.\n");

    // release shared memory.
    shmdt(p);
    shmctl(shmid, IPC_RMID, 0);
    sem_destroy(sem);  // gotta clean this stuff, no?
    exit(0);
  }
  else {
    sem_wait(sem);    // also called as "P" operation.
    printf("\nchild(%d) is in critical section.", i);
    sleep(1);

    // increment *p by 0, 1 or 2 - based on the value of "i".
    *p += i%3;
    printf("\nchild(%d) new value of *p = %d.\n", i, *p);
    sem_post(sem);    // also called as "V" operation.
    exit(0);
  }
}
