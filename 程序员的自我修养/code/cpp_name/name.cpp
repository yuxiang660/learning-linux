int func(int)
{
   return 0;
};

float func(float)
{
   return 0.0;
}

float func(int, int)
{
   return 0.0;
}

class C
{
public:
   int func(int)
   {
      return 0;
   }
   class C2
   {
   public:
      int func(int)
      {
         return 0;
      }
   };
};

namespace N
{
   int func(int)
   {
      return 0;
   }
   class C
   {
   public:
      int func(int)
      {
         return 0;
      }
   };
}

namespace foo
{
   template <typename T>
   struct Bar
   {
      void some_method(Bar<T> *one, Bar<T> *two, Bar<T> *three)
      {
         return;
      }
   };
}

void instantiate()
{
   C c_obj;
   c_obj.func(1);

   C::C2 c_c2_obj;
   c_c2_obj.func(1);

   N::C n_c_obj;
   n_c_obj.func(1);

   foo::Bar<int> foo_bar_int;
   foo_bar_int.some_method(&foo_bar_int, &foo_bar_int, &foo_bar_int);

   foo::Bar<char *> foo_bar_char;
   foo_bar_char.some_method(&foo_bar_char, &foo_bar_char, &foo_bar_char);
}
