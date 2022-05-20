#include <future>
#include <iostream>
#include <thread>

void product(std::promise<int>&& intPromise, int a, int b)
{
   intPromise.set_value(a * b);
}

struct Div
{
   void operator()(std::promise<int>&& intPromise, int a, int b) const
   {
      intPromise.set_value(a / b);
   }
};

int main()
{
   int a = 20;
   int b = 10;

   std::promise<int> prodPromise;
   std::promise<int> divPromise;

   auto prodResult = prodPromise.get_future();
   auto divResult = divPromise.get_future();

   std::thread prodThread(product, std::move(prodPromise), a, b);
   Div div;
   std::thread divThread(div, std::move(divPromise), a, b);

   std::cout << "20 * 10 = " << prodResult.get() << std::endl;
   std::cout << "20 / 10 = " << divResult.get() << std::endl;

   prodThread.join();
   divThread.join();

   return 0;
}
