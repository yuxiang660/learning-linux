#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
   int rc = fork();

   if (rc < 0) {
      fprintf(stderr, "fork failed\n");
      exit(1);
   }
   else if (rc == 0) {
      close(STDOUT_FILENO);
      open("./output.log", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

      // exec "wc"
      char* myargs[3];
      myargs[0] = strdup("wc"); // program: "wc" string at heap
      myargs[1] = strdup("redirect.c"); // argument: filename string at heap
      myargs[2] = NULL; // marks end of array
      execvp(myargs[0], myargs);
   }
   else {
      int wc = wait(NULL);
   }

   return 0;
}

