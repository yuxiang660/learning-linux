#include "./foo.h"

#include <iostream>

int main()
{
   std::cout << "Template in Header" << std::endl;
   Foo<int> f;
   std::cout << "add return " << f.add(1, 2) << std::endl;
   return 0;
}
