#include "common.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
   static int static_var = 5;
   printf("\n(%d) memory address of static_var: %p\n", getpid(), (void *)(&static_var));

   int* p = malloc(sizeof(int));
   assert(p != NULL);
   printf("(%d) memory address of p: %p\n", getpid(), p);
   *p = 0;
   while (1)
   {
      Spin(1);
      *p = *p + 1;
      printf("(%d) p: %d\n", getpid(), *p);
   }
   return 0;
}
