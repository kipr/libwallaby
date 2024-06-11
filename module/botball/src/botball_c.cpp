#include "kipr/botball/botball.h"
#include "kipr/thread/thread.hpp"
#include "kipr/button/button.h"
#include "kipr/analog/analog.h"
#include "kipr/console/console.h"
#include <kipr/console/display.h>
#include "kipr/graphics/graphics.h"
#include "kipr/graphics/graphics_characters.h"
#include <stdio.h>
#include <string.h>
#include <kipr/time/time.h>

#include "kipr/core/cleanup.hpp"

#include <cstdlib>
#include <iostream>

#ifndef WIN32
#include <unistd.h>
#endif

using namespace kipr;

class ShutDownIn : public kipr::thread::Thread
{
public:
	ShutDownIn(double s)
			: m_s(s)
	{
	}

	~ShutDownIn()
	{
	}

	virtual void run()
	{
		const double start = seconds();
		msleep(m_s * 1000.0);
		const double end = seconds();
		std::cout << std::endl << "Shutdown after " << (end - start) << " seconds" << std::endl;
		// Note: Might want to move this to botui in the future.
		// Create::instance()->stop();
		kipr::core::cleanup(true);
		_exit(0);
	}

private:
	double m_s;
};

void shut_down_in(double s)
{
	static ShutDownIn *s_instance;
	if (s_instance)
	{
		std::cout << "Warning: shut_down_in already called once. Ignoring." << std::endl;
		return;
	}
	s_instance = new ShutDownIn(s);
	s_instance->start();
}

// Waits for the light to shine on the light sensor.
// Ignores camera flashes
// Zachary Sasser - zsasser@kipr.org

// Rework of Wallaby version of wait_for_light to work with the Wombat KRC
//   cnw: 8/15/2019
#define FLASHPAUSE 100 // msecs to pause to let a flash subside
/* Assumption is that if light is detected by consecutive readings FLASHPAUSE apart,
	 it's the start light, not a flash
	 NOTE: when tested with camera, the flash persisted < 0.1 sec
*/
#define THRESHOLD 60 // minimum discrimination between light and dark
#define RED 255, 0, 0
#define GREEN 0, 255, 0
#define WHITE 255, 255, 255
#define BLACK 0, 0, 0
#define CROWS 200 // Cylon window dimensions
#define CCOLS 500
#define INSET 100 // how much to inset Cylon eye
#define DJUMP 2		// how far to jump to next red circle - affects speed of red circle
void colorbar(int i, int range);
void wait_for_light(int light_port)
{
	int OK = 0, onval, offval, reading, i = 0, range;
	while (!OK)
	{
		display_printf(0, 0, "CALIBRATE: sensor port #%d", light_port);
		display_printf(0, 1, "  press 'B' button when light is on");
		while (b_button() == 0)
		{
			msleep(100);
			onval = analog(light_port); // sensor value when light is on
			display_printf(0, 3, "  light on value is  = %4d        ", onval);
		}
		display_printf(0, 3, "  light on value is  = %4d        ", onval);
		msleep(200);
		while (b_button())
			; // debounce B button
		display_printf(0, 1, "  press 'B' button when light is off");
		display_printf(0, 2, " (OFF must exceed ON by at least %d)\n", THRESHOLD);
		while (b_button() == 0)
		{
			offval = analog(light_port);
			display_printf(0, 4, "  light off value is = %4d         ", offval);
			display_printf(0, 5, "                       ----        ");
			display_printf(0, 6, "              OFF-ON = %4d         ", offval - onval);
			msleep(100);
		}
		offval = analog(light_port); // sensor value when light is off
		display_printf(0, 4, "  light off value is = %4d         ", offval);
		display_printf(0, 6, "              OFF-ON = %4d         ", offval - onval);
		msleep(200);
		if ((offval - onval) >= THRESHOLD)
		{ // bright = small values
			OK = 1;
			display_printf(0, 8, "Good Calibration!");
			msleep(1000);
			graphics_open(CCOLS, CROWS);
			graphics_fill(WHITE);			 // paint screen
			range = CCOLS - 2 * INSET; // inset amount for green bar that follows
			graphics_rectangle_fill(INSET, CROWS / 2 - 20, CCOLS - INSET, CROWS / 2 + 20, GREEN);
			graphics_circle_fill(INSET, CROWS / 2, 20, GREEN);
			graphics_circle_fill(CCOLS - INSET, CROWS / 2, 20, GREEN);
			graphics_update();
			graphics_print_string("READING - ON = ", INSET, 30, BLACK, 2);
			while (1)
			{												// loop until light verified on
				colorbar(i++, range); // draw moving red circle
				// msleep(20); // alternate for setting red circle speed to DJUMP
				reading = analog(light_port);
				graphics_rectangle_fill(INSET + 210, 30, INSET + 270, 60, WHITE); // erase text
				graphics_print_int(reading - onval, 1, INSET + 210, 30, BLACK, 2);
				if ((reading - onval) < THRESHOLD)
				{																// reading is low enough for light on
					msleep(FLASHPAUSE);						// pause
					reading = analog(light_port); // get second reading to rule out flash
					if ((reading - onval) < THRESHOLD)
						break; // if still low enough, light is on
				}
			}
			graphics_close();
			console_clear();
			printf("Diagnostics:\n");
			printf("OFF: %d, ON: %d, OFF-ON: %d\n", offval, onval, offval - onval);
			printf("READ: %d, READ-ON=%d < THRESHOLD=%d\n", reading, reading - onval, THRESHOLD);
			printf(" (good enough to recognize lights on)\n");
		}
		else
		{
			display_printf(0, 8, "BAD CALIBRATION");
			if (offval < 256)
			{
				display_printf(15, 8, " - Add Shielding!!");
				msleep(5000);
			}
			else
			{
				display_printf(15, 8, " - Aim sensor!!");
				msleep(5000);
			}
			display_clear();
		}
	}
}
void colorbar(int i, int range)
{
	int d, limit;
	limit = range / DJUMP;		// how far to traverse before reversing
	d = i % (int)(2 * limit); // scale i to total number of jumps (half forward and half back)
	if (d < limit)
		graphics_circle_fill(INSET + DJUMP * d, CROWS / 2, 20, RED); // move to right
	else
		graphics_circle_fill(INSET + DJUMP * limit - DJUMP * (d - limit), CROWS / 2, 20, RED); // move to left
	graphics_update();
	if (d < limit)
		graphics_circle_fill(INSET + DJUMP * d, CROWS / 2, 20, GREEN); // remove circle from next iteration
	else
		graphics_circle_fill(INSET + DJUMP * limit - DJUMP * (d - limit), CROWS / 2, 20, GREEN);
}