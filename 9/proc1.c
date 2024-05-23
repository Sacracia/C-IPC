#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 128

int main(int argc, char* argv[]) {
	if (argc != 3) {
	  printf("Correct format: inputfile outputfile");
	  exit(-1);
  }
  char fifo_AtoB[] = "AtoB.fifo";
  char fifo_BtoA[] = "BtoA.fifo";
  unlink(fifo_AtoB);
  unlink(fifo_BtoA);
  char buffer[BUF_SIZE];

  if (mknod(fifo_AtoB, S_IFIFO | 0666, 0) < 0) {
    printf("Errore nella creazione del file AtoB\n");
    exit(-1);
  }
  if (mknod(fifo_BtoA, S_IFIFO | 0666, 0) < 0) {
    printf("Errore nella creazione del file BtoA\n");
    exit(-1);
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
		  printf("Fail to open file");
		  exit(-1);
	  }
  int fifo_fd = open(fifo_AtoB, O_WRONLY);
  int read_bytes;
  do {
    read_bytes = read(fd, buffer, sizeof(buffer));
    write(fifo_fd, buffer, read_bytes);
  } while (read_bytes == sizeof(buffer));
  close(fd);
  close(fifo_fd);
  fifo_fd = open(fifo_BtoA, O_RDONLY);
  char res;
  read(fifo_fd, &res, 1);
  close(fifo_fd);
  fd = open(argv[2], O_WRONLY | O_CREAT, 0644);
  if (fd < 0) {
		  printf("Fail to open file");
		  exit(-1);
	  }
  write(fd, &res, 1);
  close(fd);
  
  return 0;
}