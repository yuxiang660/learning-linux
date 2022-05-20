#include <thread>
#include <future>
#include <iostream>
#include <deque>

class SumUp
{
public:
   int operator()(int beg, int end)
   {
      int sum = 0;
      for (int i = beg; i < end; i++)
      {
         sum += i;
      }
      return sum;
   }
};

int main()
{
   SumUp s1, s2, s3, s4;
   std::packaged_task<int(int, int)> t1(s1);
   std::packaged_task<int(int, int)> t2(s2);
   std::packaged_task<int(int, int)> t3(s3);
   std::packaged_task<int(int, int)> t4(s4);

   auto f1 = t1.get_future();
   auto f2 = t2.get_future();
   auto f3 = t3.get_future();
   auto f4 = t4.get_future();

   std::deque<std::packaged_task<int(int, int)>> allTasks;
   allTasks.emplace_back(std::move(t1));
   allTasks.emplace_back(std::move(t2));
   allTasks.emplace_back(std::move(t3));
   allTasks.emplace_back(std::move(t4));

   int begin = 1;
   int increment = 250;
   int end = begin + increment;

   while (!allTasks.empty())
   {
      // fire
      std::thread sumThread(std::move(allTasks.front()), begin, end);
      allTasks.pop_front();
      begin = end;
      end += increment;
      sumThread.detach();
   }

   auto sum = f1.get() + f2.get() + f3.get() + f4.get();
   std::cout << "sum of 0 .. 1000 = " << sum << std::endl;

   return 0;
}
