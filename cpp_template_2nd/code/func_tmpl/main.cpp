#include <iostream>

template<typename T>
T max(T a, T b)
{
   return b < a ? a : b;
}

void test_max()
{
   int i = 42;
   std::cout << "max(7,i): " << max(7, i) << std::endl;
}

int main()
{
   test_max();
   return 0;
}
