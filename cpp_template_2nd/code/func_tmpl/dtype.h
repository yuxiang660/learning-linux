#pragma once

#include "./utility.h"

#include <iostream>

template<typename T>
void by_ref(T& a, T& b)
{
   std::cout << "by_ref T type: " << typeid(T).name() << std::endl;
   return;
}

template<typename T>
void by_val(T a, T b)
{
   std::cout << "by_val T type: " << typeid(T).name() << std::endl;
   return;
}

void test_type_deduction()
{
   Decorator d(__func__);

   const int a = 1;
   int b = 2;
   by_val(a, b);
   // compile error
   // by_ref(a, b);
   const int c = 3;
   by_ref(a, c);
}
