#include <thread>
#include <mutex>
#include <iostream>
#include <string>

thread_local std::string s("hello from ");
void addThreadLocal(const std::string &s2)
{
   s += s2;
   printf("%s\n&s=%p\n", s.c_str(), &s);
}

int main()
{
   std::thread t1(addThreadLocal, "t1"), t2(addThreadLocal, "t2"), t3(addThreadLocal, "t3"), t4(addThreadLocal, "t4");

   t1.join();
   t2.join();
   t3.join();
   t4.join();
   return 0;
}
