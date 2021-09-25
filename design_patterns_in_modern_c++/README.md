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
