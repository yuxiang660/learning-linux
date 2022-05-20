#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <execution>

#define NUM_SIZE 1000000

void par()
{
   std::vector<int> coll;
   coll.reserve(NUM_SIZE);
   for (int i = 0; i < NUM_SIZE; ++i)
   {
      coll.push_back(i);
   }

   auto start = std::chrono::high_resolution_clock::now();
   std::sort(std::execution::par,
             coll.begin(), coll.end(),
             [](const auto &val1, const auto &val2)
             {
                return val1 > val2;
             });
   auto end = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> diff = end - start;
   std::cout << "par elapsed time = " << diff.count() << std::endl;
}

void seq()
{
   std::vector<int> coll;
   coll.reserve(NUM_SIZE);
   for (int i = 0; i < NUM_SIZE; ++i)
   {
      coll.push_back(i);
   }

   auto start = std::chrono::high_resolution_clock::now();
   std::sort(std::execution::seq,
             coll.begin(), coll.end(),
             [](const auto &val1, const auto &val2)
             {
                return val1 > val2;
             });
   auto end = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> diff = end - start;
   std::cout << "seq elapsed time = " << diff.count() << std::endl;
}

void normal()
{
   std::vector<int> coll;
   coll.reserve(NUM_SIZE);
   for (int i = 0; i < NUM_SIZE; ++i)
   {
      coll.push_back(i);
   }

   auto start = std::chrono::high_resolution_clock::now();
   std::sort(coll.begin(), coll.end(),
             [](const auto &val1, const auto &val2)
             {
                return val1 > val2;
             });
   auto end = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> diff = end - start;
   std::cout << "normal elapsed time = " << diff.count() << std::endl;
}

int main()
{
   normal();
   par();
   seq();
   return 0;
}
