#include <future>
#include <iostream>
#include <thread>
#include <mutex>
#include <utility>

struct Div
{
   void operator()(std::promise<int>&& intPromise, int a , int b)
   {
      intPromise.set_value(a / b);
   }
};

std::mutex coutMutex;
struct Requestor
{
   void operator()(std::shared_future<int> shaFut)
   {
      std::lock_guard<std::mutex> coutGuard(coutMutex);
      std::cout << "threadId(" << std::this_thread::get_id() << "): 20/10 = " << shaFut.get() << std::endl;
   }
};

int main()
{
   std::promise<int> divPromise;
   //std::shared_future<int> sharedResult = divPromise.get_future();
   auto divResult = divPromise.get_future();
   std::cout << "divResult.valid(): " << divResult.valid() << std::endl;

   Div div;
   std::thread divThread(div, std::move(divPromise), 20, 10);

   auto sharedResult = divResult.share();
   std::cout << "divResult.valid(): " << divResult.valid() << std::endl;

   Requestor req;
   std::thread t1(req, sharedResult);
   std::thread t2(req, sharedResult);
   std::thread t3(req, sharedResult);
   std::thread t4(req, sharedResult);
   std::thread t5(req, sharedResult);

   divThread.join();
   t1.join();
   t2.join();
   t3.join();
   t4.join();
   t5.join();

   return 0;
}
