#include <future>
#include <iostream>
#include <thread>
#include <exception>
#include <string>

struct Div
{
   void operator()(std::promise<int> &&intPromise, int a, int b)
   {
      try
      {
         if (b == 0)
         {
            std::string errMess = std::string("Illegal division by zero: ") + std::to_string(a) + "/"
               + std::to_string(b);
            throw std::runtime_error(errMess);
         }
         intPromise.set_value(a / b);
      }
      catch(...)
      {
         intPromise.set_exception(std::current_exception());
      }
   }
};

void executeDivision(int nom, int denom)
{
   std::promise<int> divPromise;
   auto divResult = divPromise.get_future();

   Div div;
   std::thread divThread(div, std::move(divPromise), nom, denom);

   try
   {
      int res = divResult.get();
      std::cout << nom << "/" << denom << " = " << res << std::endl;
   }
   catch(const std::exception& e)
   {
      std::cerr << e.what() << '\n';
   }
   divThread.join();
}


int main()
{
   executeDivision(20, 0);
   executeDivision(20, 10);

   return 0;
}
