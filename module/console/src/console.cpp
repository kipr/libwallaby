#include "kipr/console/console.hpp"
#include <cstdio>

using namespace kipr;
using namespace kipr::console;

void Console::clear()
{
  printf("\f");
  fflush(stdout);
}
