#include <wallaby/compass.hpp>

#include "compass_p.hpp"


Compass::Compass()
{

}

::Compass::~Compass()
{

}

void Compass::calibrate()
{
	return Private::Compass::instance()->calibrate();
}

void Compass::setParams(float meanX, float meanY, float meanZ,
				    float W1, float W2, float div_E1, float div_E2)
{
	Private::Compass::instance()->setParams(meanX, meanY, meanZ, W1, W2, div_E1, div_E2);
}

float Compass::getAngle(void)
{
	return Private::Compass::instance()->getAngle();
}
