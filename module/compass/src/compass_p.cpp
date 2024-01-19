#include "compass_p.hpp"
#include <kipr/magneto/magneto.h>
#include <kipr/time/time.h>

#include <cstdio>
#include <cmath>

#define NSAMPLES 1000




namespace Private
{

Compass::Compass()
: mean_x(0.f),
  mean_y(0.f),
  mean_z(0.f),
  w1(0.f),
  w2(0.f),
  div_e1(0.f),
  div_e2(0.f)
{

}

Compass::~Compass()
{

}

Compass * Compass::instance()
{
	static Compass instance;
	return & instance;
}


/**
   Calibration procedure for the on-board compass.

   Assumptions: The Z axis of the magnetometer is pointing upwards
   (i.e., the wallaby is sitting flat).

   Once "Go" is printed to the console, the program expects that the robot makes
   two full turns during the calibration process.  The turning should be a consistent
   speed throughout to make sure that we achieve a regular sampling.

   Theory of operation:
   - The magnetometer vector samples form an ellipse in 3 dimensions.  Because the
   wallaby is sitting flat (or nearly flat), this ellipse is almost horizontal.  Our
   goal is to compute the parameters that are necessary to transform this ellipse into
   a circle that is sitting in the x/y plane

   Process:
   1. zero-center the data (to address magnetometer biases)
   2. transform the ellipse to a circle
   3. project the circle into the x/y plane
 */
void Compass::calibrate()
{
	  printf("Slowly turn the compass for two full cycles, keeping the compass flat...\n\r");
	  msleep(2000);

	  printf("Go\n\r");

	  // Storage for many samples
	  float x[NSAMPLES];
	  float y[NSAMPLES];
	  float z[NSAMPLES];

	  // Mean and covariance structures
	  float sum_x2, sum_y2;
	  float sum_xy, sum_xz, sum_yz;
	  float sum_x, sum_y, sum_z;
	  float det;

	  // Collect the data
	  int i;
	  for(i=0; i < NSAMPLES; ++i){
	    x[i] = magneto_x();
	    y[i] = magneto_y();
	    z[i] = magneto_z();
	    if(i% 10 == 0) printf(".");
	    msleep(20);
	  };

	  // Compute the components
	  printf("\n\rDone collecting data.  Thinking...\n\r");

	  // Compute mean
	  sum_x = sum_y = sum_z = 0;
	  for(i=0; i < NSAMPLES; ++i){
	    sum_x += x[i];
	    sum_y += y[i];
	    sum_z += z[i];
	    //printf("%f %f %f\n\r", x[i], y[i], z[i]);
	  };

	  mean_x = sum_x / NSAMPLES;
	  mean_y = sum_y / NSAMPLES;
	  mean_z = sum_z / NSAMPLES;

	  //printf("Means: %f %f %f\n\r", mean_x, mean_y, mean_z);

	  // Zero-center data
	  //printf("Zero centering...\n\r");

	  for(i=0; i < NSAMPLES; ++i){
	    x[i] -= mean_x;
	    y[i] -= mean_y;
	    z[i] -= mean_z;
	  };

	  // Compute higher-order covariance terms
	  //printf("2nd-order terms...\n\r");

	  sum_x2 = sum_y2 = sum_xy = sum_xz = sum_yz = 0.0;

	  for(i=0; i < NSAMPLES; ++i){
	    sum_x2 += x[i] * x[i];
	    sum_y2 += y[i] * y[i];
	    sum_xy += x[i] * y[i];
	    sum_xz += x[i] * z[i];
	    sum_yz += y[i] * z[i];
	  };

	  //printf("Inverting covariance matrix...\n\r");
	  det = sum_x2 * sum_y2 - sum_xy * sum_xy;

	  // Inverse is [iA, iB; iC iD]
	  float iA = sum_y2 / det;
	  float iB = -sum_xy / det;
	  float iC = -sum_xy / det;
	  float iD = sum_x2 / det;

	  // Slopes (Plane normal vector is [-w1; -w2; 1]
	  w1 =  iA * sum_xz + iB * sum_yz;
	  w2 =  iC * sum_xz + iD * sum_yz;

	  //printf("Slopes: %f %f\n\r", w1, w2);

	  // Orthogonal vectors [1; 0; w1] and [0; 1; w2]
	  div_e1 = sqrt(1 + w1*w1);
	  div_e2 = sqrt(w1*w1*w2*w2 + 1.0 + 2*w1*w1
	                + w1*w1*w1*w1 + w2*w2);

	  // Give the user the line of code that will calibrate the compass directly
	  printf("Complete: set_compass_params(%f, %f, %f, %f, %f, %f, %f)\n\r",
	         mean_x, mean_y, mean_z, w1, w2, div_e1, div_e2);
}

void Compass::setParams(float meanX, float meanY, float meanZ,
				    float W1, float W2, float div_E1, float div_E2)
{
	  mean_x = meanX;
	  mean_y = meanY;
	  mean_z = meanZ;
	  w1 = W1;
	  w2 = W2;
	  div_e1 = div_E1;
	  div_e2 = div_E2;
}

float Compass::getAngle(void)
{
	  float mag[3];

	  // Read the current magnetometer state
	  mag[0] = magneto_x();
	  mag[1] = magneto_y();
	  mag[2] = magneto_z();

	  // Adjust zero.  (is this specific to each sensor?)

	  mag[0] -= mean_x;
	  mag[1] -= mean_y;
	  mag[2] -= mean_z;

	  // Scale and project onto x/y plane
	  float p_x = (mag[0] + w1 * mag[2])/div_e1;
	  float p_y = (-mag[0] * w1 * w2 + mag[1] * (1 + w1*w1) + mag[2] * w2)/div_e2;

	  // Compute the angle
	  float orient = atan2(p_x, p_y);

	  return(orient);
}

}
