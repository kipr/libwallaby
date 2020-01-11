#include "kipr/botball.h"

#include "kipr/display.h"
#include "kipr/thread.hpp"
#include "kipr/general.h"
#include "kipr/create.hpp"
#include "kipr/compat.hpp"

#include <stdio.h>
#include <string.h>
#include <kipr/util.h>
#include <kipr/graphics.h>
#include <kipr/graphics_characters.h>
#include <kipr/console.h>
#include <kipr/digital.h>
#include <kipr/analog.h>
#include <kipr/button.h>
#include <kipr/audio.h>

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
		Private::Wallaby::atExit(true);
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
#define CROWS 200 // Cylon window dimensions
#define CCOLS 500
#define INSET 100 // how much to inset Cylon eye
#define DJUMP 2   // how far to jump to next red circle - affects speed of red circle
void colorbar(int i, int range);
void wait_for_light(int port)
{
    int badDifference = 100; //The standard for what is too close for the light values

    while(1>0){

        while(any_button()){} //Debounce

        while(!any_button()){

            printf("Turn on light and press button... \n");
	    printf("Looking for light on PORT....#%d\n", port);
            printf("Light ON Value: %d  <---- \n", analog(port));
            printf("     |=| \n");
            printf("     / \\ \n");
            printf("--- (   ) ---  \n");
            printf("   , `-'.   \n");
            printf("  /   |  \\   \n");
            printf(" '    |   `   \n");

            if(any_button()){ break; } //Makes the button more responsive

            msleep(200);
            console_clear(); //For refreshing the values

        }

        int initial = analog(port); //Set Light ON Value

        while(any_button()){} //Debounce

        while(!any_button()){
            printf("Turn off light and press button... \n \n");
	    printf("Looking for light on PORT....#%d\n", port);
            printf("Light ON Value: %d \n", initial);
            printf("Light OFF Value: %d  <---- \n", analog(port));
            printf("     |=| \n");
            printf("     / \\ \n");
            printf("    (   ) \n");
            printf("     `-' \n");

            if(any_button()){ break; } //Makes the button more responsive

            msleep(200);
            console_clear(); //For refreshing the values

        }

        int final = analog(port); //Set the light off value

        while(any_button()){} //Debounce
		
        if((final-initial)>badDifference){ //Check to make sure the values are good.
		
	     //If the light is 10% of the way to the ON value, it is a light.
            int threshold = (final-initial)*0.1+initial;
		
	     /*
		This part keeps camera flashes from starting the robot. The 2 second wait is how long my cell phone flash takes. -Zach
	     */
            while(analog(0)>threshold){
                 while(analog(0)>threshold){
                    printf("Waiting for starting light...\n \n");
		    printf("Looking for light on PORT....#%d\n", port);
                    printf("Light ON Value: %d \n", initial);
                    printf("Light OFF Value: %d \n", final);
                    printf("---------------------- \n");
                    printf("Threshold Value: %d \n \n", threshold);
                    printf("Current Value: %d  <----  \n", analog(port));
                    msleep(200);
                    console_clear();

                }
                printf("Waiting for starting light...\n \n");
		printf("Looking for light on PORT....#%d\n", port);
                printf("Light ON Value: %d \n", initial);
                printf("Light OFF Value: %d \n", final);
                printf("---------------------- \n");
                printf("Threshold Value: %d \n \n", threshold);
                printf("Current Value: %d  <----  \n", analog(port));
                msleep(2000);
                console_clear();
            }
            return;
        }

	//If the values were too close together
        else{


            console_clear();
            printf("BAD CALIBRATION!!!");         
            if(final<1000){
             	printf(" -> Shield Light Sensor. \n");  
            }
            else{
                printf(" -> Values are too close.\n");
            }
            printf(" \n \n Press any button to restart calibration.");
            while(!any_button()){} //Wait until they press the button.
        }
    }}
void colorbar(int i, int range) {
    int d, limit;
    limit = range/DJUMP; // how far to traverse before reversing
    d=i%(int)(2*limit);  // scale i to total number of jumps (half forward and half back)
         if (d<limit) graphics_circle_fill(INSET+DJUMP*d,CROWS/2,20,RED); // move to right
         else graphics_circle_fill(INSET+DJUMP*limit-DJUMP*(d-limit),CROWS/2,20,RED); // move to left
         graphics_update();
    if (d<limit) graphics_circle_fill(INSET+DJUMP*d,CROWS/2,20,GREEN); // remove circle from next iteration
         else graphics_circle_fill(INSET+DJUMP*limit-DJUMP*(d-limit),CROWS/2,20,GREEN);
}
