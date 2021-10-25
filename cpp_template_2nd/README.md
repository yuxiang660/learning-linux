# Intro
"C++ Templates - The Complete Guide" (Second Edition) Notes

## About This Book
### Goal
* to provide the definitive guide to C++ templates, including
   * a solid reference
   * an accessible tutorial

## Why Templates?
* If no template,
   * you may have to write general code for a common base type such as `Object`. Then you will lose the benefit of type checking
   * you may have to use a special preprocessor, which might result in strange semantic errors
* If having template,
   * full support of type checking
   * parameterize behavior
   * optimize code

# Function Templates
## Two-Phase Translation
* The template are compiled in two phases:
   * Without instantiation at definition time
      * check the syntax error
   * At instantiation time
      * check the template instance code with the specific type
* Compiling and Linking
   * Two phase translation break the usual compile and link rule
      * when the declaration of a function is sufficient to compile its use
   * Simple rule: **Implement each template inside a header file**

## Template Argument Deduction
### TypeConversions During Type Deduction
* limitation, see the [example](./code/func_tmpl/dtype.h)
   * by reference
      * even trivial conversions do not apply to type deduction
      * two arguments declared with the same template parameter T must match exactly
   * by value
      * only trivial conversions that decay are supported
      * two arguments declared with the same template parameter T the decayed types must match
* default arguments
   * have to specify default type
   ```cpp
   template<typename T = std::string>
   void f(T = "")
   {}
   f(); // OK
   ```

## Multiple Template Parameters
### Template Parameters for Return Types
* If a return type depends on template parameters, to deduce the return type is a good choice.
   * C++14: `auto max(int a, float b) { ... }`
* an initialization of type auto always decays
   ```cpp
   int i = 42;
   int const& ir = i;
   auto a = ir;   // a is declared as new object of type int
   // same as, std::decay<decltype(ir)>::type
   ```
# Class Templates
* templates can only be defined in global/namespace scope or inside class declarations
* C++17 introduced class argument template deduction
   * C++14 must always specify the template arguments explicitly
## Friends
* Define friend function (it is normal function) inside a template class
   ```cpp
   template<typename T>
   class Stack {
      friend std::ostream& operator<< (std::ostream& strm, Stack<T> const& s) {
         s.printOn(strm);
         return strm;
      }
   };
   ```
* Two options to declare friend function without definition in side a template class
   * declare a new function template
      ```cpp
      template <typename T>
      class Stack {
         template<typename U>
         friend std::ostream& operator<< (std::ostream&, Stack<U> const&);
      };
      ```
   * forward declare first (not recommend)

## Specializations of Class Templates
* Comparison
   * overloading for function template
   * specializing for class template
### Partial Specialization
* Two type
   * One template parameter
      * For example, `T*` for `template <typename T>`
   * Multiple template parameters
      * if more than one partial specialization matches equally well, compile error

# Nontype Template Parameters
## Restrictions for Nontype Template Parameters
* nontype template parameters can only be:
   * constant integral values, including boolean and enumerations
   * pointers to objects/functions/members
   * lvalue references to objects or functions
   * std::nullptr_t
* nontype template parameters can not only be:
   * floating-point numbers
   * class-type objects, such as: `std::string`
   * for string literals, temporaries, or data members and other sub-objects, restrictions apply
* Differences between different C++ Version
   ```cpp
   extern char const s03[] = "hi";  // external linkage
   char const s11[] = "hi";         // internal linkage
   int main()
   {
      Message<s03> m03; // OK in all version
      Message<s11> m11; // OK since C++11
      static char const s17[] = "hi";  // no linkage
      Message<s17> m17; // OK since C++17
   }
   ```

## Template Parameter Type auto
* Since C++17, a nontype template parameter allows `auto` keyword.
   * as a value
      ```cpp
      template<auto T>
      class Message {
      public:
         void print() {
            std::cout << T << std::endl;
         }
      };
      ```
   * as a reference
      ```cpp
      template<decltype(auto) N>
      class C {
         ...
      };
      int i;
      C<(i)> x; // N is int&
      ```

# Variadic Templates
* [Example](./code/func_tmpl/vtmpl.h)

## Operator sizeof...
```cpp
template<typename T, typename... Types>
void print(T firstArg, Types... args)
{
   std::cout << sizeof...(Types) << std::endl;  // print number of remaining types
   std::cout << sizeof...(args) << std::endl;   // print number of remaining args
   ...
}
```
