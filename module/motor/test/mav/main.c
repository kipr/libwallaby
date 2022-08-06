#include <stdio.h>

#include "kipr/motor/motor.h"
#include "kipr/time/time.h"

int main(int argc, char **argv)
{
  while (1)
  {
    int i;

    printf("forward\n");
    for (i = 0; i < 4; ++i)
      mav(i, 100);
    msleep(10000);

    printf("backward\n");
    for (i = 0; i < 4; ++i)
      mav(i, -100);
    msleep(10000);
  }

  return 0;
}
