# Intro
Notes about "Design Patterns in Modern C++" by Dmitri Nesteruk (Apress, 2018).

# Introduction
## The SOLID Design Principles
* Single Responsibility Principle (SRP)
   * For example, split save from Journal class
* Open-Closed Principle (OCP)
   * a type is open for extension but closed for modification
* Liskov Substitution Principle (LSP)
* Interface Segregation Principle (ISP)
   * segregate parts of a complicated interface into separate interfaces
* Dependency Inversion Principle (DIP)
   * high-level module should not depend on low-level modules, should depend on abstractions
   * abstractions should not depend on details

# Creational Patterns
* Different ways for creation
   * stack allocation
   * heap allocation using raw pointer
   * unique pointer
   * shared pointer
   * weak pointer

## Builder
### Scenario
* creation of complicated objects
* piecewise construction of a complicated object

### Composite Builder
* If we want to create Person in following way, we can use composite builder
```cpp
   Person p = Person::create()
   .lives().at("123 London Road")
            .with_postcode("SW1 1GB")
            .in("London")
   .works().at("PragmaSoft")
            .as_a("Consultant")
            .earning(10e6);
```

### Summary
   * only make sense when the construction of the object is a nontrivial process
   * simple objects should use a constructor (or dependency injection) without necessitating a Builder

## Factories
* Term factory means
   * A class that knows how to create objects
   * A function that, when called, creates an object
### Scenario
* same constructor signature but different behavior

### Summary
* A factory method is a class member that replaces a constructor
* A factory is typically a separate class that knows how to constructor objects
* Factory VS constructor call
   * A factory can say no, for example, return a nullptr
   * A single factory can make objects of many different types
   * A factory can implement caching and other storage optimizations
* Factory VS Builder
   * Factory typically create an object in one go
   * Builder constructs the object piecewise by providing information in parts

## Prototype
### Scenario
* instead of creating an entire object from scratch, take a preconstructed object and either use a copy of it, or customize if a little

### Prototype Factory
* Create with a proto
   ```cpp
   static unique_ptr<Contact> NewEmployee(string name, int suite, Contact& proto)
   {
      auto result = make_unique<Contact>(proto); // Contact should have deep copy-constructor
      result->name = name;
      result->address->suite = suite;
      return result;
   }
   ```

### Summary
* Prototype pattern requires deep copy
   * done in a copy constructor/copy assignment operator
   * or in a separate member function, such as `clone()`

## Singleton
### Scenario
* the singleton design pattern grew out of a very simple idea that you should only have one instance of a particular component in your application
   * For example, a component that loads a database into memory and offers a read-only interface

### Singleton as Global Object
* static global object: `static Database database{};`
   * the trouble with global static objects is that
      * their initialization order in different compilation units is undefined
* a global function that exposes the necessary object:
   ```cpp
   Database& get_database() {
      static Database database;
      return database;
   }
   ```

### Classic Implementation
* Having a global static Database doesn't really prevent anyone from making another instance

```cpp
struct Database
{
protected:
   Database() { /* do what you need to do */ }
public:
   static Database& get()
   {
      // thread-safe in C++11
      static Database database;
      return database;
   }
   Database(Database const&) = delete;
   Database(Database&&) = delete;
   Database& operator=(Database const&) = delete;
   Database& operator=(Database &&) = delete;
};
```

### The Trouble with Singleton
* If using a singleton directly inside a class, it may be hard to do unit test
   * We should pass in the interface of the singleton to the singleton client

### Summary
* Avoid using singleton directly
* Keep specifying singleton as a dependency (e.g., a constructor argument) for testability

# Structural Pattern
Structural patterns are all about setting up the structure of your application so as to improve SOLID:
* Inheritance
* Composition
   * implies that the child cannot exist without the parent
   * Think of an object having members of owner<T>type: when the object gets destroyed, they get destroyed with it
* Aggregation
   * an object can contain another object, but that object can also exist independently

## Adapter

### Scenario
* We are given an interface, but want a different one, and building an adapter over the interface is what gets us to where we want to be
   * adapt the geometry to pixel-based representation

### Summary
* In the process of adaptation, you sometimes end up generating temporary data (caching). Ensuring that new data is only generated when necessary

## Bridge

### The Pimpl Idiom
* Advantages of Pimpl Idiom
   * hide the class implementation
   * modifying the data members of the hidden Impl class does not affect binary compatibility
   * the header file only needs to include the header files needed for the declaration

### Summary
* The participants of the Bridge pattern do need to be aware of each other's existence, which is different from Mediator pattern or Adapter pattern

## Composite
The composite design pattern allows us to provide identical interfaces for individual objects and collections of objects. For example, if you want to use following implementation(`connect_to` for both Neuron and NeuronLayer), then composite pattern may be a choice.
```cpp
Neuron neuron, neuron2;
NeuronLayer layer, layer2;

neuron.connect_to(neuron2);
neuron.connect_to(layer);
layer.connect_to(neuron);
layer.connect_to(layer2);
```

## Decorator
### Scenario
* working with a class your colleague wrote, and you want to extend that class' functionality, but not inherit from it.
* The Decorator pattern allows us to enhance existing types without either modifying the original types (OCP) or causing explosion of the number of derived types

### Dynamic Decorator
* Purpose
   * Reduce number of inherit classes

```cpp
struct ColoredShape : Shape
{
   Shape& shape;
   string color;

   CloredShape(Shape& shape, const string& color)
      : shape{shape}, color{color} {}

   string str() const override
   {
      ostringstream oss;
      oss << shape.str() << " has the color " << color;
      return oss.str();
   }
};
```
* `ColoredShape` inherits `Shape` and also stores `Shape&` as a member, so we can constructor `ColoredShape` with other shapes

### Static Decorator
* Purpose
   * Reduce number of inherit classes

```cpp
// Usage
ColoredShape<TransparentShape<Square>> sq = { "red", 51, 5 };

template <typename T> struct TransparentShape : T
{
   uint8_t transparency;
   template<typename...Args>
   TransparentShape(const uint8_t transparency, Args ...args)
      : T(std::forward<Args>(args)...), transparency{ transparency } {}
   ...
}; // same for ColoredShape
```

### Functional Decorator
* lambda
   ```cpp
   struct Logger
   {
      function<void()> func;
      string name;

      Logger(const function<void()>& func, const string& name)
         : func{func},
         name{name}
      {
      }

      void operator()() const
      {
         cout << "Entering " << name << endl;
         func();
         cout << "Exiting " << name << endl;
      }
   };

   // Usage
   Logger([]() {cout << "Hello" << endl; }, "HelloFunction")();
   ```
* template class
   ```cpp
   template <typename R, typename... Args>
   struct Logger3<R(Args...)>
   {
      Logger3(function<R(Args...)> func, const string& name)
         : func{func},
         name{name}
      {
      }
      R operator() (Args ...args)
      {
         cout << "Entering " << name << endl;
         R result = func(args...);
         cout << "Exiting " << name << endl;
         return result;
      }

      function<R(Args ...)> func;
      string name;
   };
   ```
* template function
   ```cpp
   template <typename R, typename... Args>
   auto make_logger3(R (*func)(Args...), const string& name)
   {
      return Logger3<R(Args...)>(
      std::function<R(Args...)>(func),
      name);
   }

   // usage
   auto logged_add = make_logger3(add, "Add");
   auto result = logged_add(2, 3);
   ```

### Summary
A decorator gives a class additional functionality while adhering to the OCP.
* Dynamic decorators
   * can store references of the decorated objects and provide dynamic composability
   * at hte expense of not being able to access the underlying objects' own members
* Static decorators
   * use mixin inheritance (inheriting from template parameter) to compose decorators at compile-time
   * give access to the underlying object's members
* Functional decorators
   * can wrap either blocks of code or particular functions to allow composition of behaviors

## Facade
The Façade design pattern is a way of putting a simple interface in front of one or more complicated subsystems. In our example, a complicated set-up involving many buffers and viewports can be used directly or, if you just want a simple console with a single buffer and associated viewport, you can get it through a very accessible and intuitive API.


