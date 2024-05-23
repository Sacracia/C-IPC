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

int main(int argc, char* argv[]) {
  if (argc != 3) {
	  printf("Correct format: inputfile outputfile");
	  exit(-1);
  }
  pid_t A,B,C;
  char buffer[5000];
  char buffer2[5000];
  int read_bytes;
  char fifo_AtoB[] = "AtoB.fifo";
  char fifo_BtoA[] = "BtoC.fifo";
  unlink(fifo_AtoB);
  unlink(fifo_BtoA);

  if (mknod(fifo_AtoB, S_IFIFO | 0666, 0) < 0) {
    printf("Errore nella creazione del file AtoB\n");
    exit(-1);
  }
  if (mknod(fifo_BtoA, S_IFIFO | 0666, 0) < 0) {
    printf("Errore nella creazione del file BtoA\n");
    exit(-1);
  }

  int fork1 = fork();
  if (fork1 == 0) {
    int fork2 = fork();
    //A
    if (fork2 > 0) {
      int fd = open(argv[1], O_RDONLY);
	  if (fd < 0) {
		  printf("Fail to open file");
		  exit(-1);
	  }
      read_bytes = read(fd, buffer, sizeof(buffer));
      close(fd);
      int fifo_fd = open(fifo_AtoB, O_WRONLY);
      write(fifo_fd, buffer, read_bytes);
      close(fifo_fd);
      fifo_fd = open(fifo_BtoA, O_RDONLY);
      char res[1];
      read(fifo_fd, res, 1);
      close(fifo_fd);
      fd = open(argv[2], O_WRONLY | O_CREAT, 0644);
	  if (fd < 0) {
		  printf("Fail to open file");
		  exit(-1);
	  }
      write(fd, res, 1);
      close(fd);
    }
    // B
    else if (fork2 == 0) {
      int fifo_fd = open(fifo_AtoB, O_RDONLY);
      read_bytes = read(fifo_fd, buffer2, sizeof(buffer2));
      close(fifo_fd);
      char res = func(buffer2, read_bytes);
      fifo_fd = open(fifo_BtoA, O_WRONLY);
      write(fifo_fd, &res, 1);
      close(fifo_fd);
    }
  }
  return 0;
}