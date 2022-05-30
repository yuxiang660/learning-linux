#include "./foo.h"
#include "./bar.h"

Foo::~Foo() = default;

Foo::Foo()
{
   m_bar.reset(new Bar());
}

void Foo::foo()
{
   m_bar->bar();
   std::cout << "foo\n";
}
