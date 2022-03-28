#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>

// semun is not defined in "sys/sem.h"
union semun
{
   int val;              /* used for SETVAL only */
   struct semid_ds *buf; /* used for IPC_STAT and IPC_SET */
   ushort *array;        /* used for GETALL and SETALL */
   struct seminfo* __buf;
};

// op为-1时执行P操作，op为1时执行V操作
void pv(int sem_id, int op)
{
   struct sembuf sem_b;
   sem_b.sem_num = 0;
   sem_b.sem_op = op;
   sem_b.sem_flg = SEM_UNDO;
   semop(sem_id, &sem_b, 1);
}

int main(void)
{
   /*
   key_t key;
   if ((key = ftok(".", 'a')) == -1)
   {
      perror("ftok");
      exit(1);
   }

   int semid;
   if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1)
   {
      perror("semget");
      exit(1);
   }
   */

   int semid;
   if ((semid = semget(IPC_PRIVATE, 1, 0666)) == -1)
   {
      perror("semget");
      exit(1);
   }

   union semun arg;
   arg.val = 1;
   if (semctl(semid, 0, SETVAL, arg) == -1)
   {
      perror("semctl");
      exit(1);
   }

   pid_t id = fork();
   if (id < 0)
   {
      perror("fork");
      exit(1);
   }

   if (id == 0)
   {
      printf("child try to get binary sem\n");
      // 在父、子进程间共享IPC_PRIVATE信号量的关键就在于二者都可以操作该信号量的标识符sem_id
      pv(semid, -1);
      printf("child get the sem and would release it after 5 seconds\n");
      sleep(5);
      pv(semid, 1);
      return 0;
   }

   printf("parent try to get binary sem\n");
   pv(semid, -1);
   printf("parent get the sem and would release it after 5 seconds\n");
   sleep(5);
   pv(semid, 1);

   waitpid(id, NULL, 0);
   if (semctl(semid, 0, IPC_RMID, arg) == -1)
   {
      perror("semctl");
      exit(1);
   }

   return 0;
}
