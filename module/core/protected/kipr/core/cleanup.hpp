#ifndef _KIPR_CORE_CLEANUP_HPP_
#define _KIPR_CORE_CLEANUP_HPP_

#include <functional>

namespace kipr
{
  namespace core
  {
    typedef std::function<void ()> CleanupFunction;

    void cleanup_add(const CleanupFunction &func);
    void cleanup(bool should_abort = false);
  }
}

#endif