#include "wallaby/botball.h"

#include "wallaby/button.h"
#include "wallaby/util.h"
#include "wallaby/display.h"
#include "wallaby/analog.h"
#include "wallaby/digital.h"
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
void wait_for_light(int light_port)
{
  	int OK=0, onval, offval, reading;
	const int button_port = 13;

    set_digital_output(button_port, 0);

	while (!OK) {
		printf ("CALIBRATE: sensor port #%d\r\n", light_port);
		printf("   press ON when light is on\r\n");
		while(digital(button_port)==0){
			msleep(100);
			onval=analog(light_port); // sensor value when light is on
			printf("   light on value is = %d      \r\n", onval);
		}
		printf("   light on value is = %d       \r\n", onval);
		msleep(200);
		while (digital(button_port)); // debounce B button
		printf("   press OFF when light is off\r\n");
		while (digital(button_port)==0) {
			offval=analog(light_port);
			printf("   light off value is = %d         \r\n", offval);
			msleep(100);
		}
		offval=analog(light_port); // sensor value when light is off
		printf("   light off value is = %d         \r\n", offval);
		msleep(200);
		if ((offval-onval)>=THRESHOLD) { // bright = small values
			OK=1;
			printf("Good Calibration!\r\n");
			printf("Diff = %d:  WAITING FOR LIGHTS ON\r\n",offval-onval);
            graphics_open(480,272);
			graphics_fill(0,255,0);
          	graphics_update();

          	while (1) { // loop until light verified on
				msleep(20);
				reading=analog(light_port);
				printf("Current reading: %d \r\n", reading);
				if ((reading-onval) < THRESHOLD) { // reading is low enough for light on
					msleep(FLASHPAUSE); // pause
					reading=analog(light_port);	// get second reading to rule out flash	
				   if ((reading-onval) < THRESHOLD) break; // if still low enough, light is on
				}
			}
			printf("Reading:%4d *** LIGHTS ARE ON ***\r\n", reading);
		}
		else {
			printf("BAD CALIBRATION\r\n");
			if (offval<256) {
				printf("   Add Shielding!!\r\n");
                graphics_open(480,272);
			    graphics_fill(255,0,0);
          		graphics_update();
				msleep(5000);
			}
			else {
				printf("   Aim sensor!!\r\n");
				msleep(5000);
			}
		}

        graphics_close();
	}
}
