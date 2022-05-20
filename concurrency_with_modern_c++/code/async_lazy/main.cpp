#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int main()
{
   auto begin = std::chrono::system_clock::now();
   std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;

   auto asyncLazy = std::async(std::launch::deferred, [](){
      std::cout << "aysncLaze thread id: " << std::this_thread::get_id() << std::endl;
      return std::chrono::system_clock::now();
   });

   auto asyncEager = std::async(std::launch::async,[](){
      std::cout << "asyncEager thread id: " << std::this_thread::get_id() << std::endl;
      return std::chrono::system_clock::now();
   });

   std::this_thread::sleep_for(std::chrono::seconds(1));

   auto lazyStart = asyncLazy.get() - begin;
   auto eagerStart = asyncEager.get() - begin;

   std::cout << "asyncLazy evaluated after: "
      << std::chrono::duration<double>(lazyStart).count() << "seconds." << std::endl;
   std::cout << "asyncEager evaluated after: "
      << std::chrono::duration<double>(eagerStart).count() << "seconds." << std::endl;
   return 0;
}
