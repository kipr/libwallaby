#ifndef _KIPR_BUTTON_BUTTON_P_H_
#define _KIPR_BUTTON_BUTTON_P_H_

#include "kipr/button/button_ids.hpp"

namespace kipr
{
  namespace button
  {
    struct SharedButton;

    class Button
    {
    public:
      void setPressed(const Id &id, bool pressed);
      bool isPressed(const Id &id) const;

      void setExtraShown(const bool& shown);
      bool isExtraShown() const;

      static Button *instance();
    private:
      unsigned char buttonOffset(const Id &id) const;

      Button();
      Button(const Button& rhs);

    };
  }
}

#endif 
