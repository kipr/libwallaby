#include <stdio.h>

#include "kipr/gyro/gyro.h"
#include "kipr/time/time.h"

int main(int argc, char **argv)
{
  while (1)
  {
    signed short gx = gyro_x();
    signed short gy = gyro_y();
    signed short gz = gyro_z();

    printf("gx:%d gy:%d gz:%d\n", gx, gy, gz);
    msleep(100);
  }

  return 0;
}
