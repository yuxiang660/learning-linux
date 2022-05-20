#include <thread>
#include <mutex>
#include <iostream>

class MySingleton
{
private:
   static std::once_flag s_initInstanceFlag;
   static MySingleton *s_instance;
   MySingleton() = default;
   ~MySingleton() = default;

public:
   MySingleton(const MySingleton&) = delete;
   MySingleton& operator=(const MySingleton&) = delete;

   static MySingleton* getInstance()
   {
      std::call_once(s_initInstanceFlag, MySingleton::initSingleton);
      return s_instance;
   }

   static void initSingleton()
   {
      s_instance = new MySingleton();
   }
};

MySingleton* MySingleton::s_instance = nullptr;
std::once_flag MySingleton::s_initInstanceFlag;

void dumpSingleton()
{
   printf("MySingleton::getInstance(): %p\n", MySingleton::getInstance());
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
