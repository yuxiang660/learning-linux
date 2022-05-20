#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int main()
{
   std::async(std::launch::async,[](){
      std::cout << "thread1 id: " << std::this_thread::get_id() << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(2));
      std::cout << "first thread" << std::endl;
   });

   std::async(std::launch::async,[](){
      std::cout << "thread2 id: " << std::this_thread::get_id() << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(2));
      std::cout << "second thread" << std::endl;
   });

   // 最后才打印，程序变为顺序执行，因为发后即忘的future会在原地等待promise完成
   std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;
   std::cout << "main thread" << std::endl;
   return 0;
}
