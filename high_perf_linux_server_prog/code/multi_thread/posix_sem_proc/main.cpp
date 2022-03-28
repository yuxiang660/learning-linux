#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/mman.h>

// Refer to: https://stackoverflow.com/questions/16400820/how-to-use-posix-semaphores-on-forked-processes-in-c

void child_process(sem_t *pmutex)
{
   sem_wait(pmutex);
   printf("[Child] Entered..\n");

   // critical section
   sleep(4);

   printf("[Child] Just Exiting...\n");
   sem_post(pmutex);
}

void parent_process(sem_t *pmutex)
{
   sem_wait(pmutex);
   printf("[Parent] Entered..\n");

   // critical section
   sleep(4);

   printf("[Parent] Just Exiting...\n");
   sem_post(pmutex);
}

int main()
{
   sem_t *pmutex = (sem_t *)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
   assert(pmutex != MAP_FAILED);

   /* 1: shared across processes
    * 0: initial value, wait locked until one post happens (making it > 0)
    */
   sem_init(pmutex, 1, 1);

   pid_t pid = fork();
   if (pid == 0)
   {
      child_process(pmutex);
      return 0;
   }

   assert(pid > 0);
   parent_process(pmutex);

   wait(NULL);
   sem_destroy(pmutex);

   return 0;
}
