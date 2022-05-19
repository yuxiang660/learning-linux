#include <thread>
#include <atomic>

class SpinLock
{
   std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
   void lock()
   {
      while (flag.test_and_set()) {}
   }

   void unlock()
   {
      flag.clear();
   }
};

SpinLock spin;

void workOnResource()
{
   spin.lock();
   std::this_thread::sleep_for(std::chrono::milliseconds(2000));
   spin.unlock();
}

int main()
{
   std::thread t1(workOnResource), t2(workOnResource);

   t1.join();
   t2.join();
   return 0;
}
