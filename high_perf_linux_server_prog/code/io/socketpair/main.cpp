#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define MSGSIZE 16
const char *msg1 = "hello, world #1";
const char *msg2 = "hello, world #2";

void parent(int socket)
{
   printf("Parent Task Starts\n");

   char inbuf[MSGSIZE];
   int nbytes = read(socket, inbuf, MSGSIZE);
   printf("parent process read %d bytes to from child process: %s\n", nbytes, inbuf);

   nbytes = write(socket, msg1, MSGSIZE);
   printf("parent process wrote %d bytes to child process: %s\n", nbytes, msg1);

   printf("Parent Task Ends\n");
}

void child(int socket)
{
   printf("Child Task Starts\n");

   int nbytes = write(socket, msg2, MSGSIZE);
   printf("child process wrote %d bytes to parent process: %s\n", nbytes, msg2);

   char inbuf[MSGSIZE];
   nbytes = read(socket, inbuf, MSGSIZE);
   printf("child process read %d bytes to from parent process: %s\n", nbytes, inbuf);

   printf("Child Task Ends\n");
}

int main()
{
   printf("Test socketpair\n");

   const int parentSocket = 0;
   const int childSocket = 1;
   int fd[2];
   socketpair(PF_LOCAL, SOCK_STREAM, 0, fd);

   if (fork() > 0)
   {
      // only need parent socket in parent process
      close(fd[childSocket]);
      parent(fd[parentSocket]);
      close(fd[parentSocket]);

      printf("Finished Parent, Waiting for Child\n");
      wait(NULL);
      printf("All Done\n");
   }
   else
   {
      close(fd[parentSocket]);
      child(fd[childSocket]);
      close(fd[childSocket]);

      printf("Finished Child\n");
   }

   return 0;
}
