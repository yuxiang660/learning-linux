#include <iostream>

void crash(int i)
{
   std::cout << i << std::endl;
   int arr[2];
   arr[3] = 10;
   return;
}

int main(void)
{
   std::cout << "crash" << std::endl;
   crash(1);
   return 0;
}
