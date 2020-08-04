#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
  int fd, retval;
  char buffer[14];

  fd = open("/tmp/utkarsh_fifo", O_RDONLY);
  retval = read(fd, buffer, sizeof(buffer));
  fflush(stdin);
  write(1, buffer, sizeof(buffer));
  close(fd);

  return 0;
}
