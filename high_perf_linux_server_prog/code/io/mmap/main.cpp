#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <libgen.h> // basename
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h> // fstat
#include <string.h>   //memcpy
#include <assert.h>

int main(int argc, char *argv[])
{
   printf("Test mmap\n");

   if (argc < 3)
   {
      printf("usage: %s in_filename out_filename\n", basename(argv[0]));
      return 1;
   }
   const char *in_filename = argv[1];
   const char *out_filename = argv[2];

   int fdin = open(in_filename, O_RDONLY);
   if (fdin < 0)
   {
      printf("Error opening the file: %s\n", in_filename);
      return 1;
   }

   int fdout = open(out_filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
   if (fdout < 0)
   {
      printf("Error opening the file: %s\n", out_filename);
      return 1;
   }

   struct stat statbuf;
   int ret = fstat(fdin, &statbuf);
   assert(ret == 0);

   void *src_ptr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0);
   if (src_ptr == MAP_FAILED)
   {
      printf("Input File Mapping Failed\n");
      return 1;
   }

   // expand the space of the output file
   ret = lseek(fdout, statbuf.st_size - 1, SEEK_SET);
   assert(ret != -1);
   ret = write(fdout, "", 1);
   assert(ret == 1);

   void *dest_ptr = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0);
   if (dest_ptr == MAP_FAILED)
   {
      printf("Output File Mapping Failed\n");
      return 1;
   }

   memcpy(dest_ptr, src_ptr, statbuf.st_size);

   ret = munmap(dest_ptr, statbuf.st_size);
   assert(ret == 0);
   ret = munmap(src_ptr, statbuf.st_size);
   assert(ret == 0);

   return 0;
}
