#include "kipr/gyro/gyro.h"
#include "gyro_p.hpp"

using namespace kipr;
using namespace kipr::gyro;

short gyro_x()
{
	return gyro::gyro_x();
}

short gyro_y()
{
	return gyro::gyro_y();
}

short gyro_z()
{
	return gyro::gyro_z();
}

int gyro_calibrate()
{
	return (gyro::gyro_calibrate() ? 0 : -1);
}
