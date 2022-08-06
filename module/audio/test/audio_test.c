/*
 * audio_test.c
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */



#include <stdio.h>
#include <stdlib.h>

#include "wallaby/wallaby.h"

int main(int argc, char ** argv)
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

