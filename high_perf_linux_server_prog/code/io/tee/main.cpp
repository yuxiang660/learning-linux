#include <stdio.h>
#include <libgen.h> // basename
#include <stdlib.h> // atoi
#include <assert.h>
#include <string.h> // bzero
#include <unistd.h> // sleep, close
#include <errno.h>

#include <arpa/inet.h>
#include <fcntl.h> // tee

int main(int argc, char* argv[])
{
   printf("Test tee\n");

   if (argc != 2)
   {
      printf("usage: %s <file>\n", basename(argv[0]));
      return 1;
   }

   int filefd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
   assert(filefd > 0);

   int pipefd_stdout[2];
   int ret = pipe(pipefd_stdout);
   assert(ret != -1);

   int pipefd_file[2];
   ret = pipe(pipefd_file);
   assert(ret != -1);

   // STDIN(in) -> pipefd_stdout(write)
   ret = splice(STDIN_FILENO, NULL, pipefd_stdout[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
   assert(ret != -1);

   // pipefd_stdout(read) <--copy--> pipefd_file(write)
   ret = tee(pipefd_stdout[0], pipefd_file[1], 32768, SPLICE_F_NONBLOCK);
   assert(ret != -1);

   // pipefd_file(read) -> filefd(out)
   ret = splice(pipefd_file[0], NULL, filefd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
   assert(ret != -1);

   // pipefd_stdout(read) -> STDOUT(out)
   // Error: STDOUT_FILENO is opened in append mode, https://stackoverflow.com/questions/60016855/no-output-using-splice-syscall
   // ret = splice(pipefd_stdout[0], NULL, STDOUT_FILENO, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
   // assert(ret != -1);

   close(filefd);
   close(pipefd_stdout[0]);
   close(pipefd_stdout[1]);
   close(pipefd_file[0]);
   close(pipefd_file[1]);

   return 0;
}
