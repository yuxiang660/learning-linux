#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

#define MAXSIZE 128

struct my_msgbuf
{
   long mtype;
   char mtext[MAXSIZE];
};

void read_msg()
{
   key_t key;
   if ((key = ftok(".", 'a')) == -1)
   {
      perror("ftok");
      return;
   }

   int msqid;
   if ((msqid = msgget(key, 0444)) < 0)
   {
      perror("msgget");
      return;
   }

   msqid_ds buf;
   int res = msgctl(msqid, IPC_STAT, &buf);
   assert(res == 0);

   printf("[Read Msg] number of messages currently on queue is %lu\n", buf.msg_qnum);
   const size_t data_bytes = buf.__msg_cbytes;
   printf("[Read Msg] current number of bytes on queue is %zu\n", data_bytes);

   printf("[Read Msg] read data from message queue %d\n", key);
   struct my_msgbuf rcvbuffer;
   // Receive an answer of message type 1.
   if (msgrcv(msqid, &rcvbuffer, data_bytes, 1, 0) < 0)
   {
      perror("msgrcv");
      return;
   }

   printf("[Read Msg] %s\n", rcvbuffer.mtext);
   return;
}

int main()
{
   key_t key;
   if ((key = ftok(".", 'a')) == -1)
   {
      perror("ftok");
      return 1;
   }

   int msqid;
   if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0)
   {
      perror("msgget");
      return 1;
   }

   my_msgbuf sbuf;
   // Message Type
   sbuf.mtype = 1;

   const char data[] = "hello world";
   memcpy(sbuf.mtext, data, sizeof(data));
   size_t buflen = sizeof(data);

   if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0)
   {
      printf("Error while seding a message");
      perror("msgsnd");
      return 1;
   }

   printf("Message Sent\n");

   pid_t pid = fork();
   if (pid == 0)
   {
      read_msg();
      return 0;
   }

   assert(pid > 0);
   wait(NULL);

   if (msgctl(msqid, IPC_RMID, NULL) < 0)
   {
      perror("msgctl");
      return 1;
   }
   printf("Delete message queue %d\n", key);

   return 0;
}
