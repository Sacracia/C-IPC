#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

char func(char* str, int len) {
  int br = 0;
  for (int i = 0; i < len; ++i) {
    if (str[i] == '(')
      ++br;
    else if (str[i] == ')' && br == 0)
      return 78;
    else if (str[i] == ')')
      --br;
  }
  return 89;
}

int main(void) {
  char fifo_AtoB[] = "AtoB.fifo";
  char fifo_BtoA[] = "BtoA.fifo";
  char buffer[5000];

  int fifo_fd = open(fifo_AtoB, O_RDONLY);
  int read_bytes = read(fifo_fd, buffer, sizeof(buffer));
  close(fifo_fd);
  char res = func(buffer, read_bytes);
  fifo_fd = open(fifo_BtoA, O_WRONLY);
  write(fifo_fd, &res, 1);
  close(fifo_fd);

  return 0;
}