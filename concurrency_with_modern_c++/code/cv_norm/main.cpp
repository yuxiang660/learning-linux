#include <condition_variable>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::vector<int> mySharedWork;
std::mutex mutex_;
std::condition_variable condVar;

bool dataReady {false};

void waitingForWork()
{
   std::cout << "Waiting " << std::endl;
   std::unique_lock<std::mutex> lck(mutex_);
   condVar.wait(lck, [](){return dataReady;});
   mySharedWork[1] = 2;
   std::cout << "Work done " << std::endl;
}

void setDataReady()
{
   mySharedWork = {1, 0, 3};
   {
      std::scoped_lock<std::mutex> lck(mutex_);
      dataReady = true;
   }
   std::cout << "Data prepared" << std::endl;
   condVar.notify_one();
}

int main()
{
   std::thread t1(waitingForWork), t2(setDataReady);

   t1.join();
   t2.join();

   for (auto v : mySharedWork)
   {
      std::cout << v << " ";
   }
   std::cout << std::endl;
   return 0;
}
