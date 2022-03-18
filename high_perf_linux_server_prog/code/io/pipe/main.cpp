#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define MSGSIZE 16
const char *msg1 = "hello, world #1";
const char *msg2 = "hello, world #2";

void parent(int write_sock)
{
   printf("Parent Task Starts\n");

   int nbytes = write(write_sock, msg1, MSGSIZE);
   printf("parent process wrote %d bytes to child process: %s\n", nbytes, msg1);
   nbytes = write(write_sock, msg2, MSGSIZE);
   printf("parent process wrote %d bytes to child process: %s\n", nbytes, msg2);

   printf("Parent Task Ends\n");
}

void child(int read_sock)
{
   printf("Child Task Starts\n");

   char inbuf[MSGSIZE];
   int nbytes = 0;
   while ((nbytes = read(read_sock, inbuf, MSGSIZE)) > 0)
      printf("child process read %d bytes to from parent process: %s\n", nbytes, inbuf);

   printf("Child Task Ends\n");
}

int main()
{
   printf("Test pipe\n");

   int fd[2];
   if (pipe(fd) < 0)
   {
      printf("Fail to create the pipe\n");
      return 1;
   }

   if (fork() > 0)
   {
      // close read port because parent process only writes data
      close(fd[0]);
      parent(fd[1]);
      // write done and close it, otherwise the read op in child process will be blocked
      close(fd[1]);

      printf("Finished Parent, Waiting for Child\n");
      wait(NULL);
      printf("All Done\n");
   }
   else
   {
      // close write port because child process only reads data
      close(fd[1]);
      child(fd[0]);
      close(fd[0]);

      printf("Finished Child\n");
   }

   return 0;
}
