#include "minicrt.h"

int main(int argc, char* argv[])
{
   int i;
   int fd;
   char** v = malloc(argc*sizeof(char*));
   for (i = 0; i < argc; ++i) {
      v[i] = malloc(strlen(argv[i]) + 1);
      strcpy(v[i], argv[i]);
   }

   printf("Line number %d: %s\n", 13, "hello world");

   // const string space is limited, so stack space to store const string.
   // fopen function uses lots of const string space.
   char log[30] = "test.txt";
   char write_mode[2] = "w";
   char read_mode[2] = "r";

   fd = fopen(log, write_mode);
   for (i = 0; i < argc; ++i) {
      int len = strlen(v[i]);
      fwrite(&len, 1, sizeof(int), fd);
      fwrite(v[i], 1, len, fd);
   }
   fclose(fd);

   fd = fopen(log, read_mode);
   char format[30] = "Number:%d, String:%s\n";
   for (i = 0; i < argc; ++i) {
      int len;
      char* buf;
      fread(&len, 1, sizeof(int), fd);
      buf = malloc(len+1);
      fread(buf, 1, len, fd);
      buf[len] = '\0';
      printf(format, len, buf);
      free(buf);
      free(v[i]);
   }
   fclose(fd);

   return 0;
}
