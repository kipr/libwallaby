#include "kipr/accel/accel.hpp"
#include "accel_p.hpp"

using namespace kipr;
using namespace kipr::accel;

signed short Acceleration::x()
{
  return accel_x();
}

signed short Acceleration::y()
{
  return accel_y();
}

signed short Acceleration::z()
{
  return accel_z();
}

bool Acceleration::calibrate()
{
  return accel_calibrate();
}

short AccelX::value() const
{
  return accel_x();
}

short AccelY::value() const
{
  return accel_y();
}

short AccelZ::value() const
{
  return accel_z();
}
