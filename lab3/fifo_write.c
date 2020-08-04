#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
  int fd, retval;
  char buffer [14] = "Utkarsh Gupta\n";
  fflush(stdin);

  retval = mkfifo("/tmp/utkarsh_fifo", 0666);
  fd = open("/tmp/utkarsh_fifo", O_WRONLY);
  write(fd, buffer, sizeof(buffer));
  close(fd);

  return 0;
}
