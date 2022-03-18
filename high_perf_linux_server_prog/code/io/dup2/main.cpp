#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define MSGSIZE 17
const char *msg1 = "hello, world #1\n";
const char *msg2 = "hello, world #2\n";

int main()
{
   printf("Test dup2\n");

   int fd = open("dup2.log", O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
   if (fd < 0)
   {
      printf("Error opening the file\n");
      return 1;
   }

   // here the newfd is the file descriptor of stdout (i.e. 1)
   int stdout_fd = 1;
   dup2(fd, stdout_fd);
   printf("%s", msg1);

   write(fd, msg2, MSGSIZE);

   close(stdout_fd);
   close(fd);

   return 0;
}
