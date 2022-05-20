#include <iostream>
#include <thread>

int main()
{
   std::jthread thr([]()
                   { std::cout << "Joinable std::thread" << std::endl; });

   std::cout << "thr.joinable():" << thr.joinable() << std::endl;
   //thr.join();

   return 0;
}
