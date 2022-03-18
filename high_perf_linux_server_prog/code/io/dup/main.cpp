#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define MSGSIZE 17
const char *msg1 = "hello, world #1\n";
const char *msg2 = "hello, world #2\n";

int main()
{
   printf("Test dup\n");

   int fd = open("dup.log", O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
   if (fd < 0)
   {
      printf("Error opening the file\n");
      return 1;
   }

   int copy_fd = dup(fd);
   write(copy_fd, msg1, MSGSIZE);

   write(fd, msg2, MSGSIZE);

   close(copy_fd);
   close(fd);

   return 0;
}
