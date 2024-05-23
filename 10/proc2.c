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

int main(int argc, char* argv[]) {
  int res = 1;
  message_buf  rbuf, sbuf;
  key_t key = 10;
  int msqid = msgget(key, 0666);
  int msqid_send = msgget(20, 0666);
  
  int read_bytes;
  do {
    msgrcv(msqid, &rbuf, BUF_SIZE, 1, 0);
    res = res & func(rbuf.mtext, strlen(rbuf.mtext));
  } while (strlen(rbuf.mtext) == BUF_SIZE - 1);
  // Отправка ответа
  sbuf.mtype = 1;
  sbuf.mtext[0] = res ? 'Y' : 'N';
  msgsnd(msqid_send, &sbuf, 1, IPC_NOWAIT);
  return 0;
}