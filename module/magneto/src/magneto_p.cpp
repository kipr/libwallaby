#include "magneto_p.hpp"

#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"

using namespace kipr;
using namespace kipr::magneto;

using kipr::core::Platform;

/*
 * Outputs the internal magneto x reading
 * Pulls magnetometer data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
 */
float local_magneto_x()
{
    return Platform::instance()->readRegisterFloat(REG_RW_MAG_X_0);
}

/*
 * Outputs the internal magneto y reading
 * Pulls magnetometer data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
 */
float local_magneto_y()
{
    return Platform::instance()->readRegisterFloat(REG_RW_MAG_Y_0);
}

/*
 * Outputs the internal magneto z reading
 * Pulls magnetometer data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
 */
float local_magneto_z()
{
    return Platform::instance()->readRegisterFloat(REG_RW_MAG_Z_0);
}

/*
 * Returns the magneto_x in the NED frame. +x is forward (North). -x is backward (South).
 */
short kipr::magneto::magneto_x()
{
    return static_cast<short>(local_magneto_x());
}

/*
 * Returns the magneto_y in the NED frame. +y is right (East). -y is left (West).
 */
short kipr::magneto::magneto_y()
{
    return static_cast<short>(local_magneto_y());
}

/*
 * Returns the magneto_z in the NED frame. +z is up. -z is down.
 */
short kipr::magneto::magneto_z()
{
    return static_cast<short>(-local_magneto_z());
}
