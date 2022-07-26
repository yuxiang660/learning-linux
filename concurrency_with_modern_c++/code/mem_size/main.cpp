#include <iostream>
#include <bitset>
#include <bit>
#include <string>
#include <stdio.h>

class Empty
{
};

class A
{
   char a;
};

class B
{
   char a;
   int i;
};

class C
{
   char a;
   int b;
   double c;
};

class D
{
   char a;
   double b;
   int c;
};

class Base
{
};

class Child1 : public Base
{
public:
   int foo()
   {
      return 0;
   }
   char b;
};

class Child2: public Base
{
public:
   virtual int foo()
   {
      return 0;
   }
   char b;
};

int main()
{
   Empty e;
   std::cout << "Size of empty class Empty: " << sizeof(e) << std::endl;

   A a;
   std::cout << "Size of empty class A: " << sizeof(a) << std::endl;

   B b;
   std::cout << "Size of empty class B: " << sizeof(b) << std::endl;

   C c;
   std::cout << "Size of empty class C: " << sizeof(c) << std::endl;

   D d;
   std::cout << "Size of empty class D: " << sizeof(d) << std::endl;

   Child1 c1;
   std::cout << "Size of empty Child1: " << sizeof(c1) << std::endl;

   Child2 c2;
   std::cout << "Size of empty Child2: " << sizeof(c2) << std::endl;

   return 0;
}
