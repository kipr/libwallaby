#ifndef _COMPASS_H_
#define _COMPASS_H_

#ifdef __cplusplus
extern "C" {
#endif

void calibrate_compass();

void set_compass_params(float meanX, float meanY, float meanZ,
			    float W1, float W2, float div_E1, float div_E2);

float get_compass_angle(void);


#ifdef __cplusplus
}
#endif


#endif // #define _COMPASS_H_
