#include "./bar.h"
#include <iostream>

Bar::Bar():
   m_a(5)
{}

void Bar::bar()
{
   std::cout << "a=" << m_a << " in bar\n";
}
