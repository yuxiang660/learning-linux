#include <iostream>
#include <bitset>
#include <bit>
#include <string>
#include <stdio.h>

// __attribute__((packed, aligned(<len_of_the_max_member>)))
struct S
{
   char a;     // memory location #1
   int b : 5;  // memory location #2
   int c : 11, // memory location #2 （bitfield continue）
         : 0,  // 宽度为0的域强制下一位域，对齐到下面成员类型的边界
       d : 8;  // memory location #3
   int e;      // memory location #4
   double f;   // memory location #5
   std::string g; // several memory locations
};

#define GET_BIT_FIELD_WIDTH(T, f) \
   []()                           \
   {                              \
      T t{};                      \
      t.f = ~0;                   \
      unsigned int bitCount = 0;  \
      while (t.f != 0)            \
      {                           \
         t.f <<= 1;               \
         ++bitCount;              \
      }                           \
      return bitCount;            \
   }()

int main()
{
   S s;
   std::cout << "Byte size of S.a: " << sizeof(s.a) << std::endl;
   std::cout << "Bitfield of integral number: " << std::endl;
   std::cout << "  Bit size of S.b: " << GET_BIT_FIELD_WIDTH(S, b) << std::endl;
   std::cout << "  Bit size of S.c: " << GET_BIT_FIELD_WIDTH(S, c) << std::endl;
   std::cout << "  Bit size of S.d: " << GET_BIT_FIELD_WIDTH(S, d) << std::endl;
   std::cout << "Byte size of S.e: " << sizeof(s.e) << std::endl;
   std::cout << "Byte size of S.f: " << sizeof(s.f) << std::endl;
   std::cout << "Byte size of S.g: " << sizeof(s.g) << std::endl;
   std::cout << "Total bytes of S: " << sizeof(S) << std::endl;

   return 0;
}
