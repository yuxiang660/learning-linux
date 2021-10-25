#pragma once

#include "./utility.h"

void print() {}
template<typename T, typename... Types>
void print(T firstArg, Types... args)
{
   std::cout << firstArg << std::endl;
   print(args...);
}

template<typename T, typename... Types>
void print2(T firstArg, Types... args)
{
   std::cout << firstArg << std::endl;
   // compile error without constexpr
   // because print(args...) still is instantiated for no argument because it is a run-time decision
   // and no function print() for no arguments provided, this is an error
   if constexpr (sizeof...(args) > 0) {
      print2(args...);
   }
}

void test_variadic_template()
{
   Decorator d(__func__);

   print("d", "c", 2);
   print2("a", "b", 1);
}
