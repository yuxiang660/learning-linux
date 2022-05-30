#pragma once

#include <iostream>
#include <memory>
class Bar;
class Foo
{
public:
   Foo();
   ~Foo(); // implement when bar is complete, or use shared_ptr
   void foo();
private:
   std::unique_ptr<Bar> m_bar;
};
