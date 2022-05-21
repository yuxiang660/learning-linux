#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <iostream>

struct Sum
{
#ifdef NOFALSE
   alignas(64) long long a = 0;
   alignas(64) long long b = 0;
#else
   long long a = 0;
   long long b = 0;
#endif
};

#define SIZE 10000000

int main()
{
   Sum sum;
   printf("&sum.a=%p\n", &sum.a);
   printf("&sum.b=%p\n", &sum.b);

   std::vector<int> randVals1;
   std::vector<int> randVals2;
   randVals1.reserve(SIZE);
   randVals2.reserve(SIZE);

   int rand;
   for (long long i = 0; i < SIZE; i++)
   {
      rand = random();
      randVals1.push_back(rand);
      randVals2.push_back(rand);
   }

   auto start = std::chrono::steady_clock::now();

   std::thread t1([&sum, &randVals1](){
      for (auto val : randVals1)
         sum.a += val;
   });

   std::thread t2([&sum, &randVals2](){
      for (auto val : randVals2)
         sum.b += val;
   });

   t1.join();
   t2.join();

   std::chrono::duration<double> dur = std::chrono::steady_clock::now() - start;
   std::cout << "Time for addition " << dur.count() << " seconds\n";

   std::cout << "sum.a: " << sum.a << std::endl;
   std::cout << "sum.b: " << sum.b << std::endl;

   return 0;
}
