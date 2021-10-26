#include "./foo.h"

template <typename T>
T Foo<T>::add(T l, T r)
{
   return l+r;
}

// template specialization
template int Foo<int>::add(int l, int r);

// template<> int Foo<int>::add(int l, int r)
// {
//    return l-r;
// }
