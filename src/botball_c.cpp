#include "wallaby/botball.h"

#include "wallaby/button.h"
#include "wallaby/util.h"
#include "wallaby/display.h"
#include "wallaby/analog.h"
#include "wallaby/audio.h"
#include "wallaby/graphics.h"
#include "wallaby/thread.hpp"
#include "wallaby/general.h"
#include "wallaby/create.hpp"
#include "wallaby/compat.hpp"

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
		Create::instance()->stop();
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

// Reworked for flash suppression January 6, 2015
// Graphics signal for waiting
#define FLASHPAUSE 100  // msecs to pause to let a flash subside
/* Assumption is that if light is detected by consecutive readings FLASHPAUSE apart, 
   it's the start light, not a flash
   NOTE: when tested with camera, the flash persisted < 0.1 sec
*/
#define THRESHOLD 60    // minimum discrimination between light and dark
#define RED 255,0,0
#define GREEN 0,255,0
void colorbar(int i);
VI void wait_for_light(int light_port)
{
	int xBut, OK=0, onval, offval, reading, cb=0, i; 
	xBut=get_extra_buttons_visible(); // in case user has extra buttons visible
	set_extra_buttons_visible(0);     //    turn them off
	set_a_button_text("-");
	set_c_button_text("-");
	while (!OK) {
		set_b_button_text("Light is ON");
		display_clear();
		display_printf (0,0,"CALIBRATE: sensor port #%d", light_port);
		display_printf(0,1,"   press ON when light is on");
		while(b_button()==0){
			msleep(100);
			onval=analog(light_port); // sensor value when light is on
			display_printf(0,2,"   light on value is = %d        ", onval);
		}
		set_b_button_text("Light is OFF");
		display_printf(0,1,"   light on value is = %d        ", onval);
		msleep(200);
		beep();
		while (b_button()); // debounce B button
		display_printf(0,1,"   press OFF when light is off");
		while (b_button()==0) {
			offval=analog(light_port);
			display_printf(0,3,"   light off value is = %d         ", offval);
			msleep(100);
		}
		offval=analog(light_port); // sensor value when light is off
		display_printf(0,3,"   light off value is = %d         ", offval);
		msleep(200);
		if ((offval-onval)>=THRESHOLD) { // bright = small values
			OK=1;
			display_printf(0,4,"Good Calibration!");
			display_printf(0,7,"Diff = %d:  WAITING FOR LIGHTS ON",offval-onval);
			graphics_open(240,25);
			graphics_fill(255,255,255);
			graphics_update();
			while (1) { // loop until light verified on
				colorbar((i++)%74);
				msleep(20);
				reading=analog(light_port);
				display_printf(0,8,"Current reading: %d ", reading);
				if ((reading-onval) < THRESHOLD) { // reading is low enough for light on
					msleep(FLASHPAUSE); // pause
					reading=analog(light_port);	// get second reading to rule out flash	
				   if ((reading-onval) < THRESHOLD) break; // if still low enough, light is on
				}
			}
			graphics_close();
			display_printf(0,8,"Reading:%4d *** LIGHTS ARE ON ***", reading);
		}
		else {
			display_printf(0,7,"BAD CALIBRATION");
			if (offval<256) {
				display_printf(0,8,"   Add Shielding!!");
				msleep(5000);
			}
			else {
				display_printf(0,8,"   Aim sensor!!");
				msleep(5000);
			}
		}
	}
	set_a_button_text("A"); set_b_button_text("B"); set_c_button_text("C"); 
	set_extra_buttons_visible(xBut); // in case user had extra buttons visible
}
void colorbar(int i) {
	graphics_rectangle_fill(12,0,227,23,GREEN);
	graphics_circle_fill(11,11,12,GREEN);
	graphics_circle_fill(228,11,12,GREEN);
	if (i<37) graphics_circle_fill(11+6*i,11,12,255,0,0);
	else graphics_circle_fill(6+6*(74-i),11,12,255,0,0);
	graphics_update();
}
