#include "wallaby/botball.h"

#include "wallaby/button.h"
#include "wallaby/util.h"
#include "wallaby/display.h"
#include "wallaby/analog.h"
#include "wallaby/digital.h"
#include "wallaby/audio.h"
#include "wallaby/graphics.h"
#include "wallaby/graphics_characters.h"
#include "wallaby/thread.hpp"
#include "wallaby/general.h"
#include "wallaby/create.hpp"
#include "wallaby/compat.hpp"
#include "wallaby/console.h"

#include "wallaby_p.hpp"

#include <cstdlib>
#include <iostream>

#ifndef WIN32
#include <unistd.h>
#endif

class ShutDownIn : public Thread
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
		//Create::instance()->stop();
		Private::Wallaby::atExit();
		_exit(0);
	}
	
private:
	double m_s;
};

VI void shut_down_in(double s)
{
	static ShutDownIn *s_instance;
	if(s_instance) {
		std::cout << "Warning: shut_down_in already called once. Ignoring." << std::endl;
		return;
	}
	s_instance = new ShutDownIn(s);
	s_instance->start();
}

// Rework of Link version of wait_for_light to work on the current Wallaby implementation
// *** calls functions in bg.c for drawing Botguy
#define FLASHPAUSE 100  // msecs to pause to let a flash subside
/* Assumption is that if light is detected by consecutive readings FLASHPAUSE apart,
   it's the start light, not a flash
   NOTE: when tested with camera, the flash persisted < 0.1 sec
*/
#define THRESHOLD 60  // minimum discrimination between light and dark
#define RED 255,0,0
#define GREEN 0,255,0
#define WHITE 255,255,255
#define BLACK 0,0,0
#define INSET 150 // how much to inset Cylon eye
#define DJUMP 6   // how far to jump to next red circle

void colorbar(int i, int range);

void wait_for_light(int light_port)
{
	int xBut, OK=0, onval, offval, reading, i=0, range;
	//xBut=get_extra_buttons_visible(); // in case user has extra buttons visible
	//set_extra_buttons_visible(0);     //    turn them off
	//set_a_button_text("-");
	//set_c_button_text("-");
    display_clear();
	while (!OK) {
		//set_b_button_text("Light is ON");
		//display_clear();
		display_printf(0,0,"CALIBRATE: sensor port #%d", light_port);
		//display_printf(0,1,"   press ON when light is on");
        display_printf(0,1,"   press R button when light is on");
		//while(b_button()==0){
        while (digital(13)==0) {
			msleep(100);
			onval=analog(light_port); // sensor value when light is on
			display_printf(0,3,"   light on value is  = %4d        ", onval);
		}
		//set_b_button_text("Light is OFF");
		display_printf(0,3,"   light on value is  = %4d        ", onval);
		msleep(200);
		beep();
		//while (b_button()); // debounce B button
        while (digital(13));  // debounce
		//display_printf(0,1,"   press OFF when light is off");

        display_printf(0,1,"   now press R button when light is off");
        display_printf(0,2,"  (OFF must exceed ON by at least %d)\n", THRESHOLD);
		//while (b_button()==0) {
        while (digital(13)==0) {
			offval=analog(light_port);
			display_printf(0,4,"   light off value is = %4d         ", offval);
            display_printf(0,5,"                        ----        ");
            display_printf(0,6,"             OFF - ON = %4d         ",offval-onval);
			msleep(100);
		}
		offval=analog(light_port); // sensor value when light is off
		display_printf(0,4,"   light off value is = %4d         ", offval);
        display_printf(0,6,"               OFF-ON = %4d         ", offval-onval);
		msleep(200);
		if ((offval-onval)>=THRESHOLD) { // bright = small values
			OK=1;
			display_printf(0,8,"Good Calibration!");
            msleep(1000);
			graphics_open(480,260);
			graphics_fill(WHITE);  // black full screen
            range=480-2*INSET; // inset amount for green bar that follows
            graphics_rectangle_fill(INSET,120,480-INSET,140,GREEN);
            graphics_circle_fill(INSET,130,10,GREEN);
            graphics_circle_fill(480-INSET,130,10,GREEN);
			graphics_update();
            g_printString((char *)"READING-ON=",50,50,BLACK,1.5);
			while (1) { // loop until light verified on
				colorbar(i++, range); // draw moving red circle
				//msleep(20);
				reading=analog(light_port);
                graphics_rectangle_fill(170,50,250,70,WHITE);
                g_printInt(reading-onval,1,170,50,BLACK,1.5);
				//display_printf(0,8,"Current reading: %d ", reading);
				if ((reading-onval) < THRESHOLD) { // reading is low enough for light on
					msleep(FLASHPAUSE); // pause
					reading=analog(light_port);	// get second reading to rule out flash
				   if ((reading-onval) < THRESHOLD) break; // if still low enough, light is on
				}
			}
			graphics_close();
            console_clear();
            printf("OFF value: %d, ON value: %d, DIFFERENCE: %d\n", offval, onval, offval-onval);
            printf("READING: %d, READING-ON = %d is < THRESHOLD of %d\n",reading, reading-onval, THRESHOLD);
            printf("  (good enough to recognize lights are on)\n");
			//display_printf(0,8,"Reading:%4d *** LIGHTS ARE ON ***", reading);
		}
		else {
			display_printf(0,8,"BAD CALIBRATION");
			if (offval<256) {
				display_printf(0,9,"   Add Shielding!!");
				msleep(5000);
			}
			else {
				display_printf(0,9,"   Aim sensor!!");
				msleep(5000);
			}
            display_clear();
		}
	}
	//set_a_button_text("A"); set_b_button_text("B"); set_c_button_text("C");
	//set_extra_buttons_visible(xBut); // in case user had extra buttons visible
}


void colorbar(int i, int range)
{
    int d, limit;
    limit = range/DJUMP; // how far to traverse before reversing
    d=i%(int)(2*limit);  // scale i to total number of jumps (half forward and half back)
	 if (d<limit) graphics_circle_fill(INSET+DJUMP*d,130,10,RED); // move to right
	 else graphics_circle_fill(INSET+DJUMP*limit-DJUMP*(d-limit),130,10,RED); // move to left
	 graphics_update();
    if (d<limit) graphics_circle_fill(INSET+DJUMP*d,130,10,GREEN); // remove circle from next iteration
	 else graphics_circle_fill(INSET+DJUMP*limit-DJUMP*(d-limit),130,10,GREEN);
}
