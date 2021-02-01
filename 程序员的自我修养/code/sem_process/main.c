#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

int compteur = 0;
sem_t *sem;

int main()
{
   void *ret;
   sem = sem_open("/sem", O_CREAT, 0644, compteur);
   /*
    https://stackoverflow.com/questions/16035732/process-synchronization-with-c-language
    Not work for sem_init(sem, 0, 0);
    If pshared has the value 0, then the semaphore is shared between the threads of a process.
    If pshared is nonzero, then the semaphore is shared between processes.
   */
   sem_init(sem, 1, 0);

   pid_t pid;
   pid = fork();
   switch (pid)
   {
   case -1:
      printf("Erreur: echec du fork()\n");
      exit(1);
      break;
   case 0:
      /* PROCESSUS FILS */
      printf("Processus fils : pid = %d\n", getpid());
      sem_post(sem);
      break;
   default:
      sem_wait(sem);
      /* PROCESSUS PERE */
      printf("Ici le pere%d: le fils a un pid=%d\n", getpid(), pid);
      printf("Fin du pere.\n");
   }
}