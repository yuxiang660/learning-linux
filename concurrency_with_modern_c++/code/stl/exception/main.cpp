#include <algorithm>
#include <execution>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

int main()
{
   std::vector<int> myVec{1, 2, 3, 4, 5};

   try
   {
      std::for_each(myVec.begin(), myVec.end(), [](int)
                    { throw std::runtime_error("Without excution policy"); });
   }
   catch (const std::exception &e)
   {
      std::cerr << e.what() << '\n';
   }

   try
   {
      std::for_each(std::execution::seq, myVec.begin(), myVec.end(), [](int)
                    { throw std::runtime_error("With eexcution policy"); });
   }
   catch (const std::exception &e)
   {
      std::cerr << e.what() << '\n';
   }
   catch (...)
   {
      std::cout << "Catch-all execptions" << std::endl;
   }

   return 0;
}