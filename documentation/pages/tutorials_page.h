/**
 * @file tutorials_page.h
 * @author Eliot Hall
 * @brief a page filled with tutorials
 * @version 0.1
 * @date 2022-08-27
 *
 * @copyright Copyright (c) 2022
 *
 * @page Tutorials Tutorials
 *
 * @section module_tutorials Module Tutorials
 * - @ref Analog Analog Tutorial
 *
 * @section complete_tutorials Comprehensive Tutorials
 * - @subpage getting_started
 * - @subpage sensors_tutorial
 */

/**
 * @page getting_started Getting Started
 * \image html botguy-red-stroke-235x300.png
 * ~Hewwo~ and welcome to KIPR's libwallaby tutorial. This tutorial assumes that you know a bit of C. If you don't, there are many
 * online resources for learning C.
 *
 * Now, let's dive in.
 *
 * Let's start with a simple robot with two wheels (depicted with W's) and two motors (depicted with M's) as shown.
 *
 * ```
 *      ----------
 *      |        |
 *      |        |
 *      W-M    M-W
 *      |        |
 *      |        |
 *      ----------
 * ```
 *
 * In order to move our robot forward, we would first plug
 * in the motors to ports 0 and 1. Then, run the following code snippet.
 *
 * ```
 * #include <kipr/wombat.h>  // this is the file with all the kipr functions
 * #include <stdio.h>        // this is the file that allows for output
 *
 * int main(){
 *      printf("Hello World! This is the Getting Started tutorial.\n");
 *
 *      // define our variables
 *      int left_motor = 0;   // the port of the left motor
 *      int right_motor = 1;  // the port of the right motor
 *
 *      // move forward
 *      motor(left_motor, 100); // move at 100% forward
 *      motor(right_motor, -100);  // move at 100% backward since this motor is facing the opposite direction as the left motor
 *      msleep(1000);  // wait 1 second
 *
 *      // stop moving
 *      ao();
 *
 *
 *      printf("Congratulations! You finished the getting started tutorial.\n");
 *      return 0;
 * }
 * ```
 *
 * Just like that, you've got your robot moving!
 * We'll continue in the next tutorial with distance sensors and line following.
 */

/**
 * @page sensors_tutorial Sensors Tutorials
 * Hewwo and welcome to the sensors tutorial!
 * First, we'll start off with a detail on the types of sensors that there are.
 * @section sensor_types Sensor Types
 * @subsection analog_sensors Analog Sensors
 * \image html analog_sensors.png
 * As the above image details, there are 5 primary types of analog sensors. These get plugged into the
 * wombat's (or wallaby's) analog ports.
 * - ET sensors - These are "distance" sensors; they return values based on the distance from the sensor's lens to the nearest object.
 * The higher the value, the closer it is to an object. Note that these only detect objects in the direction of the lens.
 * - Tophat sensors and Small Tophat sensors - These sensors should be positioned facing the board. Their use is to detect the brightness of the floor,
 * which means that most teams use them to detect whether there is tape on the board or not. Essentially, these are the sensors
 * that you'll use if you want to line follow tape on the board.
 * - Light sensors - These are used for detecting the starting light. @see wait_for_light
 * - Linear Slide sensors - These are slideable sensors. They read higher values the more that they are pushed in. Common use for these would be to
 * get within a certain distance of a wall.
 * @subsection digital_sensors Digital Sensors
 * \image html digital_sensors.png
 * As the above image details, there are 3 primary types of digital sensors. These get plugged into
 * the wombat's (or wallaby's) digital ports.
 * - Lever - Also called a click sensor. Outputs 0 when not clicked and 1 when clicked.
 * - Small Touch and Large Touch - These are buttons. They output 0 when not pressed and 1 when pressed.
 *
 */