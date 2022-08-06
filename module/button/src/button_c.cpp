#include "kipr/button/button.h"
#include "kipr/button/button.hpp"

using namespace kipr;
using namespace kipr::button;

void set_a_button_text(const char * text)
{
  A.setText(text);
}

void set_b_button_text(const char * text)
{
  B.setText(text);
}

void set_c_button_text(const char * text)
{
  C.setText(text);
}

void set_x_button_text(const char * text)
{
  X.setText(text);
}

void set_y_button_text(const char * text)
{
  Y.setText(text);
}

void set_z_button_text(const char * text)
{
  Z.setText(text);
}

int button(AbstractButton * button)
{
  return button->isPressed() ? 1 : 0;
}

int a_button()
{
  return ::button(&A);
}

int b_button()
{
  return ::button(&B);
}

int c_button()
{
  return ::button(&C);
}

int x_button()
{
  return ::button(&X);
}

int y_button()
{
  return ::button(&Y);
}

int z_button()
{
  return ::button(&Z);
}

int side_button()
{
  return right_button();
}

int push_button()
{
  return ::button(&Right);
}

int left_button()
{
  return ::button(&Left);
}

int right_button()
{
  return ::button(&Right);
}

int black_button()
{
  return right_button();
}

int button_clicked(AbstractButton * button)
{
  return button->isClicked() ? 1 : 0;
}

int a_button_clicked()
{
  return button_clicked(&A);
}

int b_button_clicked()
{
  return button_clicked(&B);
}

int c_button_clicked()
{
  return button_clicked(&C);
}

int x_button_clicked()
{
  return button_clicked(&X);
}

int y_button_clicked()
{
  return button_clicked(&Y);
}

int z_button_clicked()
{
  return button_clicked(&Z);
}

int side_button_clicked()
{
  return right_button();
}

int any_button()
{
  for (unsigned char i = 0; i < 8; ++i) if (all[i]->isPressed()) return 1;
  return 0;
}

void extra_buttons_show()
{
  set_extra_buttons_visible(1);
}

void extra_buttons_hide()
{
  set_extra_buttons_visible(0);
}

int get_extra_buttons_visible()
{
  return ExtraButtons::isShown() ? 1 : 0;
}

void set_extra_buttons_visible(int visible)
{
  ExtraButtons::setShown(visible == 0 ? false : true);
}
