/*!
 * \file digital.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup digital Digitals
 */
/**
 * \page digital
 * The functions here allow for reading the values of digital sensors.
 * \section example_digital_touch
 * Assuming that there is a touch sensor plugged into digital port 0,
 * then the following code would move forward until the touch sensor
 * was clicked.
 * ```
 * #include <kipr/wombat.h>
 * 
 * int main(){
 *      int touch_port_number = 0;
 * 
 *      while (digital(touch_port_number) != 1){
 *          // move
 *          motor(0, 100);
 *          motor(1, -100);
 *      }
 * 
 *      // stop moving
 *      ao();
 *      return 0;
 * }
 * ```
 * \ingroup digital
 */
#ifndef _KIPR_DIGITAL_DIGITAL_H_
#define _KIPR_DIGITAL_DIGITAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \see get_digital_value
 * \ingroup digital
 */
int digital(int port);

/*!
 * Sets the value of the digital port in output mode.
 * \see get_digital_value
 * \ingroup digital
 */
void set_digital_value(int port, int value);


/*!
 * Gets the current value of the digital port.
 * \return 1 if the switch is closed, 0 if the switch is open
 * \see set_digital_value
 * \ingroup digital
 */
int get_digital_value(int port);

/*!
 * Sets the digital mode.
 * \param[in] port The port to modify.
 * \param[in] out 1 for output mode, 0 for input mode.
 * \ingroup digital
 */
void set_digital_output(int port, int out);

/*!
 * Gets the current digital mode
 * \return 1 for output mode, 0 for input mode
 * \see set_digital_value
 * \ingroup digital
 */
int get_digital_output(int port);

/*!
 * Gets the current digital pullup state
 * \return 1 for active, 0 for inactive
 * \deprecated not applicable on the Wallaby
 * \ingroup digital
 */
int get_digital_pullup(int port);

/*!
 * Sets the current digital pullup state
 * \param[in] port The port to modify
 * \param[in] pullup The pullup state 1: active  0: inactive
 * \deprecated not applicable on the Wallaby
 * \ingroup digital
 */
void set_digital_pullup(int port, int pullup);


#ifdef __cplusplus
}
#endif

#endif
