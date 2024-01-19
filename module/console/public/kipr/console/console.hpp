/*!
 * \file console.hpp
 * \brief Classes for working with the console screen
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup console Console
 */

#ifndef _KIPR_CONSOLE_CONSOLE_HPP_
#define _KIPR_CONSOLE_CONSOLE_HPP_

#include "kipr/export/export.h"

namespace kipr
{
  namespace console
  {
    /*!
    * \class Console
    * Methods for working with the console inside the UI
    * \ingroup console
    */
    class EXPORT_SYM Console
    {
    public:
      /*!
      * Clears the console.
      */
      static void clear();
    };
  }
}


#endif
