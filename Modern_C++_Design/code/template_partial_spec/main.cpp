#include <iostream>

template <typename T, typename U> class Widget
{
public:
   void Fun() {
      std::cout << "template Fun()" << std::endl;
   }
};

// Member Specialization
template<> void Widget<char, int>::Fun()
{
   std::cout << "template specialization Fun()" << std::endl;
}

/* Member Partial Specialization is not supported
template<typename U> void Widget<char, U>::Fun()
{
   std::cout << "template partial specialization Fun()" << std::endl;
}
*/

template <typename T, typename U> T Fun(U obj)
{
   std::cout << "template namespace-level function" << std::endl;
   return T();
}

template <typename T> T Fun(int obj)
{
   std::cout << "template namespace-level overloading" << std::endl;
   return T();
}

template <> int Fun<int, char>(char obj)
{
   std::cout << "template namespace-level full specialization" << std::endl;
   return 0;
}

/* Compile error because method doesn't support partial specialization
template <class U> void Fun<void, U>(U obj)
{
   std::cout << "template namespace-level partial specialization" << std::endl;
}*/

int main()
{
   Widget<int, char> w;
   w.Fun();

   Widget<char, int> w2;
   w2.Fun();

   double a = 1;
   int b = Fun<int>(a);
   Fun<int, char>(1);
   Fun<char>(1);

   return 0;
}
