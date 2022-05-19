#include <memory>
#include <thread>
#include <iostream>
#include <atomic>
#include <vector>

int main()
{
   std::shared_ptr<int> ptr = std::make_shared<int>(2011);
   std::vector<std::thread> threads;
   for (int i = 0; i < 10; i++)
   {
      // not safe
      // std::thread([&ptr]{
      //    ptr = std::make_shared<int>(2014);
      // }).detach();

      // safe one
      threads.emplace_back([&ptr]{
         auto localPtr = std::make_shared<int>(2014);
         std::atomic_store(&ptr, localPtr);
         ptr = std::make_shared<int>(2014);
      });
   }

   std::cout << *ptr << std::endl;

   for (auto &t : threads)
   {
      t.join();
   }

   return 0;
}
