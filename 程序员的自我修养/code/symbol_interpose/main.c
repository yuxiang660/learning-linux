#include <stdio.h>

void b1();
void b2();

void c() {
   printf("c main.c\n");
}

int main()
{
   b1();
   b2();
   c();
   return 0;
}
