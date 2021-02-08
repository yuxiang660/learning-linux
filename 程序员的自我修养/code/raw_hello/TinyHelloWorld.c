/*
   https://github.com/chenpengcong/blog/issues/13
   int write(int filedesc, char* buffer, int size);
   0x80 -> interrupt
   eax -> system call write(4)
   ebx -> filedesc handler(1)
   ecx -> buffer(str)
   edx -> str size(13)
*/

char *str = "Hello World!\n";

void print()
{
   asm("movl $13,%%edx \n\t"
       "movl %0,%%ecx \n\t"
       "movl $1,%%ebx \n\t"
       "movl $4,%%eax \n\t"
       "int $0x80 \n\t" ::"r"(str)
       : "edx", "ecx", "ebx");
}

void exit()
{
   asm("movl $43,%ebx \n\t"
       "movl $1,%eax \n\t"
       "int $0x80 \n\t");
}

void nomain()
{
   print();
   exit();
}