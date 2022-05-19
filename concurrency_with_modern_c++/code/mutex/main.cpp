#include <thread>
#include <mutex>

std::mutex mut;

void workOnResource()
{
   mut.lock();
   std::this_thread::sleep_for(std::chrono::milliseconds(2000));
   mut.unlock();
}

int main()
{
   std::thread t1(workOnResource), t2(workOnResource);

   t1.join();
   t2.join();
   return 0;
}
