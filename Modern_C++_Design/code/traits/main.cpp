#include <iostream>
#include <vector>

class NullType;

template <typename T>
class TypeTraits
{
private:
   template <class U> struct PointerTraits
   {
      enum { result = false };
      typedef NullType PointeeType;
   };
   template <class U> struct PointerTraits<U*>
   {
      enum { result = true };
      typedef U PointeeType;
   };

   template <class U> struct PToMTraits
   {
      enum { result = false };
   };
   template <class U, class V>
   struct PToMTraits<U V::*>
   {
      enum { result = true };
   };

   template <class U> struct UnConst
   {
      typedef U Result;
   };
   template <class U> struct UnConst<const U>
   {
      typedef U Result;
   };

public:
   enum { isPointer = PointerTraits<T>::result };
   typedef typename PointerTraits<T>::PointeeType PointeeType;

   enum { isMemberPointer = PToMTraits<T>::result };

   // Remove const from the T
   typedef typename UnConst<T>::Result NonConstType;
};

int main()
{
   const bool iterIsPtr = TypeTraits<std::vector<int>::iterator>::isPointer;
   std::cout << "vector<int>::iterator is " << (iterIsPtr ? "fast" : "smart") << std::endl;
   return 0;
}
