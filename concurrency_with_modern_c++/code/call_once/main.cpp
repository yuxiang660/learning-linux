#include <thread>
#include <mutex>
#include <iostream>

std::once_flag onceFlag;

void do_once()
{
   std::call_once(onceFlag, [] {std::cout << "Only once." << std::endl;});
}

void do_once2()
{
   std::call_once(onceFlag, [] {std::cout << "Only once2." << std::endl;});
}

int main()
{
   // "Only once" 或 "Only once2"只显示一次
   std::thread t1(do_once), t2(do_once), t3(do_once2), t4(do_once2);

   t1.join();
   t2.join();
   t3.join();
   t4.join();
   return 0;
}
