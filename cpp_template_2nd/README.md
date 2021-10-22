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





