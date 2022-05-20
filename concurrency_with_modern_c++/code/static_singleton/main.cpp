#include <thread>
#include <mutex>
#include <iostream>

class MySingleton
{
private:
   MySingleton() = default;
   ~MySingleton() = default;

   MySingleton(const MySingleton&) = delete;
   MySingleton& operator=(const MySingleton&) = delete;

public:
   static MySingleton& getInstance()
   {
      static MySingleton instance;
      return instance;
   }
};

void dumpSingleton()
{
   printf("MySingleton::getInstance(): %p\n", &MySingleton::getInstance());
}

int main()
{
   std::thread t1(dumpSingleton), t2(dumpSingleton), t3(dumpSingleton), t4(dumpSingleton);

   t1.join();
   t2.join();
   t3.join();
   t4.join();
   return 0;
}
