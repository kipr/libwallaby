#include <stdio.h>

#include "kipr/accel/accel.h"
#include "kipr/time/time.h"

int main(int argc, char **argv)
{
  while (1)
  {
    signed short ax = accel_x();
    signed short ay = accel_y();
    signed short az = accel_z();

    printf("ax:%d ay:%d az:%d\n", ax, ay, az);

    msleep(100);
  }

  return 0;
}
