#include "accel_p.hpp"
#include "kipr/core/platform.hpp"
#include "kipr/core/registers.hpp"
#include "kipr/time/time.h"

constexpr float gravity = 9.80665;

using namespace kipr;
using namespace kipr::accel;

using kipr::core::Platform;

namespace
{
    double biasX = 0;
    double biasY = 0;
    double biasZ = 0;
}

/*
 * Outputs the internal accel x reading
 * Pulls accelerometer data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
 */
float local_accel_x()
{
    return Platform::instance()->readRegisterFloat(REG_RW_ACCEL_X_0);
}

/*
 * Outputs the internal accel y reading
 * Pulls accelerometer data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
 */
float local_accel_y()
{
    return Platform::instance()->readRegisterFloat(REG_RW_ACCEL_Y_0);
}

/*
 * Outputs the internal accel z reading
 * Pulls accelerometer data from the I2C registers. The MPU 9250 outputs high and low registers that need to be combined.
 */
float local_accel_z()
{
    return Platform::instance()->readRegisterFloat(REG_RW_ACCEL_Z_0);
}

/*
 * Returns the accel_x in the NED frame. +x is forward (North). -x is backward (South).
 */
short kipr::accel::accel_x()
{
    return static_cast<short>(-local_accel_y());
}

/*
 * Returns the accel_y in the NED frame. +y is right (East). -y is left (West).
 */
short kipr::accel::accel_y()
{
    return static_cast<short>(local_accel_x());
}

/*
 * Returns the accel_z in the NED frame. +z is up. -z is down.
 */
short kipr::accel::accel_z()
{
    return static_cast<short>(local_accel_z());
}

bool kipr::accel::accel_calibrate()
{
    constexpr int samples = 50;
    constexpr int delay_ms = 10;

    // Find the average readings when device is stationary
    double sumX = 0, sumY = 0, sumZ = 0;
    
    // Collect samples
    for (int i = 0; i < samples; ++i)
    {
        sumX += local_accel_x();
        sumY += local_accel_y();
        sumZ += local_accel_z();
        msleep(delay_ms);
    }

    // Calculate average raw values
    const double avgX = sumX / samples;
    const double avgY = sumY / samples;
    const double avgZ = sumZ / samples;
    
    // Determine which axis has the largest absolute value (likely pointing toward/away from gravity)
    double absX = std::abs(avgX);
    double absY = std::abs(avgY);
    double absZ = std::abs(avgZ);
    
    // Autodetect gravity direction and calibrate
    if (absZ >= absX && absZ >= absY) {
        const double gravitySign = avgZ >= 0 ? 1.0 : -1.0;
        
        biasX = avgX;
        biasY = avgY;
        biasZ = avgZ - gravitySign * gravitySign;
        printf("[Accel Calibration]: Z axis is aligned with gravity (%f)\n", gravitySign);
    } 
    else if (absX >= absY && absX >= absZ) {
        const double gravitySign = avgX >= 0 ? 1.0 : -1.0;
        
        biasX = avgX - gravitySign * gravitySign;
        biasY = avgY;
        biasZ = avgZ;
        printf("[Accel Calibration]: X axis is aligned with gravity (%f)\n", gravitySign);
    }
    else {
        const double gravitySign = avgY >= 0 ? 1.0 : -1.0;
        
        biasX = avgX;
        biasY = avgY - gravitySign * gravitySign;
        biasZ = avgZ;
        printf("[Accel Calibration]: Y axis is aligned with gravity (%f)\n", gravitySign);
    }

    printf("[Accel Calibration]: Bias X: %f | Bias Y: %f | Bias Z: %f\n", biasX, biasY, biasZ);
    return true;  // Return true to indicate successful calibration
}