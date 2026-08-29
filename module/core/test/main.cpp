#include "kipr/core/cleanup.hpp"
#include <iostream>

int main()
{
  std::cout << "Calling kipr::core::cleanup()..." << std::endl;
  kipr::core::cleanup(false);
  std::cout << "cleanup() returned without crashing." << std::endl;
  return 0;
}
