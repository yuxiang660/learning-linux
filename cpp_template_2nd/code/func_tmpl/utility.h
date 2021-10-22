#pragma once

#include <iostream>
#include <string>

class Decorator
{
public:
   explicit Decorator(const char* name):
      m_name(name)
   {
      std::cout << "--- " << m_name << " Start ---" << std::endl;
   }

   ~Decorator()
   {
      std::cout << "--- " << m_name << " End ---" << std::endl;
   }

private:
   std::string m_name;
};
