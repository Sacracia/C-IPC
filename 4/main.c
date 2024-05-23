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
  int fd_AtoB[2];
  int fd_BtoC[2];
  char buffer[5000];
  char buffer2[5000];
  int read_bytes;

  if (pipe(fd_AtoB) == -1) {
    printf("Errore nella creazione della pipe AtoB\n");
    exit(-1);
  }
  if (pipe(fd_BtoC) == -1) {
    printf("Errore nella creazione della pipe BtoC\n");
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
      close(fd_AtoB[0]);
      write(fd_AtoB[1], buffer, read_bytes);
      close(fd_AtoB[1]);
    }
    else if (fork2 == 0) {
      int fork3 = fork();
      // B
      if (fork3 > 0) {
        close(fd_AtoB[1]);
        read_bytes = read(fd_AtoB[0], buffer2, sizeof(buffer2));
        char res = func(buffer2, read_bytes);
        close(fd_BtoC[0]);
        write(fd_BtoC[1], &res, 1);
        close(fd_BtoC[1]);
      }
      // C
      else {
        close(fd_BtoC[1]);
        char r[1];
        read(fd_BtoC[0], r, 1);
        close(fd_BtoC[0]);
        int fd = open(argv[2], O_WRONLY | O_CREAT, 0644);
		if (fd < 0) {
		  printf("Fail to open file");
		  exit(-1);
		}
        write(fd, r, 1);
        close(fd);
      }
    }
  }
  return 0;
}