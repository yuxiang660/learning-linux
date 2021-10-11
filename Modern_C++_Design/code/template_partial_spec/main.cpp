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

int main()
{
   std::cout << "template example" << std::endl;

   Widget<int, char> w;
   w.Fun();

   Widget<char, int> w2;
   w2.Fun();

   return 0;
}
