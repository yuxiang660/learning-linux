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
* Scenario
   * creation of complicated objects
   * piecewise construction of a complicated object
* Composite Builder
   * If we want to create Person in following way, we can use composite builder
   ```c
    Person p = Person::create()
      .lives().at("123 London Road")
              .with_postcode("SW1 1GB")
              .in("London")
      .works().at("PragmaSoft")
              .as_a("Consultant")
              .earning(10e6);
   ```
* Summary
   * only make sense when the construction of the object is a nontrivial process
   * simple objects should use a constructor (or dependency injection) without necessitating a Builder

