#include <thread>
#include <future>
#include <iostream>
#include <vector>

void calcProducts(std::packaged_task<int(int, int)> &task,
   const std::vector<std::pair<int, int>> &pairs)
{
   for (auto &pair : pairs)
   {
      auto fut = task.get_future();
      task(pair.first, pair.second);
      // 同步执行完成后，才可以reset
      task.reset();
      std::cout << pair.first << " * " << pair.second << " = " << fut.get() << std::endl;
   }
}


int main()
{
   std::vector<std::pair<int, int>> allPairs;
   allPairs.push_back({1, 2});
   allPairs.push_back({2, 3});
   allPairs.push_back({3, 4});
   allPairs.push_back({4, 5});

   std::packaged_task<int(int, int)> task{[](int fir, int sec) {return fir * sec;}};
   calcProducts(task, allPairs);

   std::thread t(calcProducts, std::ref(task), allPairs);
   t.join();

   return 0;
}
