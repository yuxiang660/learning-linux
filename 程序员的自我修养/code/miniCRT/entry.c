#include "minicrt.h"

extern int main(int argc, char* argv[]);
void exit(int);

static void crt_fatal_error(const char* msg)
{
   exit(1);
}

/*
*  Entry Function of the Process
*  1. init
*  2. main
*  3. clean
*/
void mini_crt_entry(void)
{
   int ret;

   // Per stack layout, get argc and argv after context switching.
   // For details, refer to Chapter10 about stack layout.
   int argc;
   char** argv;
   char* ebp_reg = 0;
   // ebp_reg = %ebp
   asm("movl %%ebp,%0 \n":"=r"(ebp_reg));
   argc = *(int*)(ebp_reg+4);
   argv = (char**)(ebp_reg+8);

   if (!mini_crt_heap_init()) crt_fatal_error("heap initialize failed");
   if (!mini_crt_io_init()) crt_fatal_error("IO initialize failed");

   ret = main(argc, argv);
   exit(ret);
}

void exit(int exitCode)
{
   asm("movl %0,%%ebx \n\t"
       "movl $1,%%eax \n\t"
       "int $0x80     \n\t"
       "hlt           \n\t"::"m"(exitCode));
}
