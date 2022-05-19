#include <atomic>
#include <iostream>

template <typename T>
T fetch_mult(std::atomic<T> &shared, T mult)
{
   T oldValue = shared.load();
   while (!shared.compare_exchange_strong(oldValue, oldValue * mult)) {}
   return oldValue;
}

template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
std::atomic<T>& operator*=(std::atomic<T> &t1, T t2)
{
   T expected = t1.load();
   while (!t1.compare_exchange_strong(expected, expected * t2)) {}
   return t1;
}

int main()
{
   std::atomic<int> myInt{5};
   std::cout << myInt << std::endl;
   fetch_mult(myInt, 5);
   std::cout << myInt << std::endl;
   // Wrong
   // myInt = myInt * 10;
   myInt *= 10;
   std::cout << myInt << std::endl;
   return 0;
}
