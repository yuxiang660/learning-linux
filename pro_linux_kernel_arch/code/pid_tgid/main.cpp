#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>

#include <vector>

void *thread_start(void *arg)
{
   pthread_t id = pthread_self();
   printf("[Thread - 0x%lx] <Kernel-PGID>:getppid()=%d, <Kernel-TGID>:getpid()=%d, <Kernel-PID | LWP>:gettid()=%d\n",
      id, getppid(), getpid(), gettid());
   return nullptr;
}

int main(void)
{
   int pid = fork();
   if (pid == 0)
   {
      printf("[Child Process Main - 0x%lx] <Kernel-PGID>:getppid()=%d, <Kernel-TGID>:getpid()=%d, <Kernel-PID | LWP>:gettid()=%d\n",
         pthread_self(), getppid(), getpid(), gettid());

      pthread_t id1;
      pthread_t id2;
      pthread_create(&id1, NULL, &thread_start, NULL);
      pthread_create(&id2, NULL, &thread_start, NULL);
      pthread_join(id1, nullptr);
      pthread_join(id2, nullptr);
      printf("[Thread1 0x%lx] Done\n", id1);
      printf("[Thread2 0x%lx] Done\n", id2);

      return 0;
   }

   wait(nullptr);
   printf("[Parent Process Main - 0x%lx] <Kernel-PGID>:getppid()=%d, <Kernel-TGID>:getpid()=%d, <Kernel-PID | LWP>:gettid()=%d\n",
      pthread_self(), getppid(), getpid(), gettid());

   return 0;
}
