#include <stdio.h>

void __attribute__((constructor)) init_function(void)
{
   printf("so constructor\n");
}


void __attribute__((destructor)) fini_function(void)
{
   printf("so destructor\n");
}


void foo()
{
   printf("foo\n");
}
