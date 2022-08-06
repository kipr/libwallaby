#include <kipr/compass/compass.h>
#include <kipr/compass/compass.hpp>

using namespace kipr;
using namespace kipr::compass;

void calibrate_compass()
{
  Compass::calibrate();
}

void set_compass_params(float meanX, float meanY, float meanZ,
                        float W1, float W2, float div_E1, float div_E2)
{
  Compass::setParams(meanX, meanY, meanZ, W1, W2, div_E1, div_E2);
}

float get_compass_angle(void)
{
  return Compass::getAngle();
}
