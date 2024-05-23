#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 128

int br = 0;

int func(char* str, int len) {
  for (int i = 0; i < len; ++i) {
    if (str[i] == '(')
      ++br;
    else if (str[i] == ')' && br == 0)
      return 0;
    else if (str[i] == ')')
      --br;
  }
  return 1;
}

int main(void) {
  int res = 1;
  char fifo_AtoB[] = "AtoB.fifo";
  char fifo_BtoA[] = "BtoA.fifo";
  char buffer[BUF_SIZE];
  int fifo_fd = open(fifo_AtoB, O_RDONLY);
  int read_bytes;
  do {
    read_bytes = read(fifo_fd, buffer, sizeof(buffer));
    res = res && func(buffer, read_bytes);
  } while (read_bytes == sizeof(buffer));
  close(fifo_fd);
  fifo_fd = open(fifo_BtoA, O_WRONLY);
  char r = res ? 'Y' : 'N';
  write(fifo_fd, &r, 1);
  close(fifo_fd);
}