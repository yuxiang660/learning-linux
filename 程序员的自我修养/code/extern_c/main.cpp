#include <stdio.h>

namespace myname {
   int var = 42;
   int var2 = 43;
}

extern "C" int _ZN6myname3varE;
extern "C" int var3;
int var3 = 44;
int var4 = 45;


extern "C" int func(int);

int func(int)
{
   return 0;
}

float func(float)
{
   return 0;
}

int main()
{
   printf("%d\n", _ZN6myname3varE);
   printf("%d\n", myname::var2);
   printf("%d\n", var3);
   printf("%d\n", var4);
   return 0;
}
