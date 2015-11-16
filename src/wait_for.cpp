/*
 * wait_for.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */


#include "wallaby/wait_for.h"
#include "wallaby/button.h"
#include "wallaby/util.h"
#include "wallaby/digital.h"

VI void wait_for_milliseconds(long msecs)
{
  msleep(msecs);
}

VI void wait_for_touch(int port)
{
  while (!digital(port)) msleep(10);
}

VI void wait_for_a_button()
{
  while (!a_button()) msleep(10);
}

VI void wait_for_b_button()
{
  while (!b_button()) msleep(10);
}

VI void wait_for_c_button()
{
  while (!c_button()) msleep(10);
}

VI void wait_for_x_button()
{
  while (!x_button()) msleep(10);
}

VI void wait_for_y_button()
{
  while (!y_button()) msleep(10);
}

VI void wait_for_z_button()
{
  while (!z_button()) msleep(10);
}

VI void wait_for_side_button()
{
  while (!side_button()) msleep(10);
}

VI void wait_for_any_button()
{
  while (!any_button()) msleep(10);
}

VI void wait_for_a_button_clicked()
{
  while (!a_button_clicked()) msleep(10);
}

VI void wait_for_b_button_clicked()
{
  while (!b_button_clicked()) msleep(10);
}

VI void wait_for_c_button_clicked()
{
  while (!c_button_clicked()) msleep(10);
}

VI void wait_for_x_button_clicked()
{
  while (!x_button_clicked()) msleep(10);
}

VI void wait_for_y_button_clicked()
{
  while (!y_button_clicked()) msleep(10);
}

VI void wait_for_z_button_clicked()
{
  while (!z_button_clicked()) msleep(10);
}

VI void wait_for_side_button_clicked()
{
  while (!side_button_clicked()) msleep(10);
}
