#include <algorithm>
#include <execution>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <mutex>

std::mutex m;

int main()
{
   std::vector<int> v{1, 2, 3, 4, 5};

   int sum = 0;
   std::for_each(std::execution::par, v.begin(), v.end(), [&sum](int i)
                 {
                     std::lock_guard<std::mutex> lock(m);
                     sum += i; });

   std::cout << "sum = " << sum << std::endl;
   return 0;
}
