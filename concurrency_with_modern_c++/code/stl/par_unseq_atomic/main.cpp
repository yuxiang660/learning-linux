#include <algorithm>
#include <execution>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <mutex>
#include <atomic>

std::mutex m;

int main()
{
   std::vector<int> v{1, 2, 3, 4, 5};

   // 必须是原子类型，不然`par_unseq`会产生死锁，因为可能同一线程连续两次调用`m.lock`，后一次锁一直等待前一次释放锁
   std::atomic<int> sum = 0;
   std::for_each(std::execution::par_unseq, v.begin(), v.end(), [&sum](int i)
                 {
                     std::lock_guard<std::mutex> lock(m);
                     sum += i + i; });

   std::cout << "sum = " << sum << std::endl;
   return 0;
}
