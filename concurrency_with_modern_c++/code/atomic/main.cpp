#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> x;
std::atomic<int> y;

void run1()
{
   x.store(1);
   int res1 = y.load();
   printf("res1 = %d\n", res1);
}

void run2()
{
   y.store(1);
   int res2 = x.load();
   printf("res2 = %d\n", res2);
}

int main()
{
   std::thread t1(run1), t2(run2);

   t1.join();
   t2.join();
   return 0;
}
