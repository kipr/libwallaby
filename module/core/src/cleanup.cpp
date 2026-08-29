#include "kipr/core/cleanup.hpp"

#include <vector>
#include <cstdlib>
#include <mutex>
#include <iostream>

using namespace kipr;
using namespace kipr::core;

namespace
{
  std::vector<CleanupFunction> &cleanup_functions()
  {
    static std::vector<CleanupFunction> *functions = new std::vector<CleanupFunction>();
    return *functions;
  }
}

std::mutex &kipr::core::cleanup_mutex()
{
  static std::mutex *mutex = new std::mutex();
  return *mutex;
}

void kipr::core::cleanup_add(const CleanupFunction &func)
{
  std::lock_guard<std::mutex> lock(cleanup_mutex());

  cleanup_functions().push_back(func);
}

void kipr::core::cleanup(bool should_abort)
{
  std::lock_guard<std::mutex> lock(cleanup_mutex());

  std::cout << "Cleaning up " << cleanup_functions().size() << std::endl;

  auto &funcs = cleanup_functions();
  for (auto it = funcs.rbegin(); it != funcs.rend(); ++it) {
    if (*it) {
      (*it)();
    }
  }
  funcs.clear();  

  if (should_abort) abort();
}