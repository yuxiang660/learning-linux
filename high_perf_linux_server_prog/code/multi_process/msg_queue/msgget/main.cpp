#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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
   printf("created message queue %d by process %d\n", key, getpid());

   if (msgctl(msqid, IPC_RMID, NULL) < 0)
   {
      perror("msgctl");
      return 1;
   }
   printf("deleted message queue %d by process %d\n", key, getpid());

   return 0;
}
