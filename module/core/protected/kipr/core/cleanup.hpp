#ifndef _KIPR_CORE_CLEANUP_HPP_
#define _KIPR_CORE_CLEANUP_HPP_

#include <functional>
#include <mutex>

namespace kipr
{
  namespace core
  {
    typedef std::function<void ()> CleanupFunction;

    extern std::mutex cleanup_mutex;
    void cleanup_add(const CleanupFunction &func);
    void cleanup(bool should_abort = false);
  }
}

#endif