#include "kipr/gyro/gyro.hpp"
#include "gyro_p.hpp"

using namespace kipr;
using namespace kipr::gyro;

signed short Gyro::x()
{
	return gyro_x();
}

signed short Gyro::y()
{
	return gyro_y();
}

signed short Gyro::z()
{
	return gyro_z();
}

bool Gyro::calibrate()
{
	return gyro_calibrate();
}

short GyroX::value() const
{
	return gyro_x();
}

short GyroY::value() const
{
	return gyro_y();
}

short GyroZ::value() const
{
	return gyro_z();
}
