#include <stdio.h>

#include "kipr/magneto/magneto.h"
#include "kipr/time/time.h"

int main(int argc, char **argv)
{
  while (1)
  {
    signed short mx = magneto_x();
    signed short my = magneto_y();
    signed short mz = magneto_z();

    printf("mx:%d my:%d mz:%d\n", mx, my, mz);

    msleep(100);
  }

  return 0;
}
