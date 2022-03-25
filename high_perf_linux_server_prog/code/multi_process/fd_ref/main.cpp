#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

/*
 * Q: How to check the file descriptor reference count?
 * A: Set break point at child process, and use `lsof test.log`
 *  1. gdb ./main
 *  2. > set follow-fork-mode child
 *  3. > b main.cpp:29
 *  4. > r
 *  5. break at child process code, then `lsof test.log` in another terminal
*/
int main()
{
   printf("Test fork file descriptor copy\n");

   int fd = open("fd.log", O_CREAT | O_WRONLY, 0744);
   assert(fd > 0);

   int pid = fork();
   if (pid == 0)
   {
      // child process
      printf("[Child Process] fd = %d\n", fd);

      const char buf[] = "write from child process\n";
      write(fd, buf, sizeof(buf));

      close(fd);
      return 0;
   }

   assert(pid > 0);
   wait(NULL);
   printf("[Parent Process] fd = %d\n", fd);

   const char buf[] = "write from parent process\n";
   write(fd, buf, sizeof(buf));

   close(fd);
   return 0;
}
