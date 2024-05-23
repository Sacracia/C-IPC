#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 128

typedef struct msgbuf {
  long mtype;
  char mtext[BUF_SIZE];
} message_buf;

int main(int argc, char* argv[]) {
	if (argc != 3) {
	  printf("Correct format: inputfile outputfile");
	  exit(-1);
  }
  message_buf sbuf;
  key_t key = 10;
  int msgflg = IPC_CREAT | 0666;
  size_t buf_length;
  
  int msqid = msgget(key,  msgflg);
  int msqid_recv = msgget(20, IPC_CREAT | 0666);
  sbuf.mtype = 1;
  int readBytes;
  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
		  printf("Fail to open file");
		  exit(-1);
	  }
  do {
    readBytes = read(fd, sbuf.mtext, BUF_SIZE - 1);
    sbuf.mtext[readBytes] = '\0';
    msgsnd(msqid, &sbuf, readBytes + 1, IPC_NOWAIT);
  } while (readBytes == BUF_SIZE - 1);
  close(fd);
  // Запись ответа
  fd = open(argv[2], O_WRONLY | O_CREAT, 0644);
  if (fd < 0) {
		  printf("Fail to open file");
		  exit(-1);
	  }
  msgrcv(msqid_recv, &sbuf, 1, 1, 0);
  write(fd, sbuf.mtext, 1);
  close(fd);
  return 0;
}