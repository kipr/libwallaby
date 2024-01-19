#include <stdio.h>
#include <stdlib.h>

#include "kipr/audio/audio.h"
#include "kipr/time/time.h"

int main(int argc, char **argv)
{
  int i;
  for (i = 0; i < 3; ++i)
  {
    printf("beep!");
    beep();
    msleep(1000);

    system("aplay /usr/share/battlehill/turn_off_wallaby.wav");
    msleep(1000);
  }

  return 0;
}
