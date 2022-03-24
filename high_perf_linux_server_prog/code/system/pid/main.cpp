#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

int main()
{
   printf("Test pid gpid\n");

   int pid = fork();
   if (pid == 0)
   {
      // child process
      printf("child pid=%d, group pid=%d\n", getpid(), getpgid(getpid()));
      return 0;
   }

   // parent process wait for child process end
   assert(pid > 0);
   wait(NULL);
   printf("main pid=%d, group pid=%d\n", getpid(), getpgid(getpid()));

   return 0;
}
