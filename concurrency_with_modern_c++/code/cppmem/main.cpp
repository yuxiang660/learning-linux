#include <iostream>
#include <thread>

int x = 0;
int y = 0;

void writing()
{
   x = 1;
   y = 1;
}

void reading()
{
   printf("y = %d\n", y);
   printf("x = %x\n", x);
}

int main()
{
   std::thread t1(writing);
   std::thread t2(reading);
   t1.join();
   t2.join();

   return 0;
}
