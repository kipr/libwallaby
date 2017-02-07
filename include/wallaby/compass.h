#ifndef _COMPASS_H_
#define _COMPASS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \file compass.h
 * \author Joshua southerland
 * \copyright KISS Institute for Practical Robotics
 * \defgroup compass Compass
 */


/*!
 * Begin calibrating the compass.
 *
 * Will display the calibration parameter results which
 * can be used in the future with set_compass_params
 *
 * Provided by Dr. Andrew H. Fagg and Twister Robotics
 * \ingroup compass
 */
void calibrate_compass();


/*!
 * Set the compass parameters based on a previous calibration.
 *
 * Provided by Dr. Andrew H. Fagg and Twister Robotics
 *
 * \param[in] meanX a value provided by calibrate_compass
 * \param[in] meanY a value provided by calibrate_compass
 * \param[in] meanZ a value provided by calibrate_compass
 * \param[in] W1 a value provided by calibrate_compass
 * \param[in] W2 a value provided by calibrate_compass
 * \param[in] div_E1 a value provided by calibrate_compass
 * \param[in] div_E2 a value provided by calibrate_compass
 * \ingroup compass
 */
void set_compass_params(float meanX, float meanY, float meanZ,
			    float W1, float W2, float div_E1, float div_E2);

/*!
 * Get the current compass heading.
 *
 * Provided by Dr. Andrew H. Fagg and Twister Robotics
 * \return the compass angle in radians
 * \ingroup compass
 */
float get_compass_angle(void);


#ifdef __cplusplus
}
#endif


#endif // #define _COMPASS_H_
