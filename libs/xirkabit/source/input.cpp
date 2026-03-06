#include "pxt.h"
#include <math.h>

using namespace pxt;

namespace input {

int accelX() {
    if (!accelerometer)
        return 0;
    return accelerometer->getX();
}

int accelY() {
    if (!accelerometer)
        return 0;
    return accelerometer->getY();
}

int accelZ() {
    if (!accelerometer)
        return 0;
    return accelerometer->getZ();
}

int accelStrength() {
    int x = accelX();
    int y = accelY();
    int z = accelZ();
    // compute magnitude (use 64-bit to avoid overflow)
    int64_t sum = (int64_t)x * x + (int64_t)y * y + (int64_t)z * z;
    int result = (int)sqrt((double)sum);
    return result;
}

} // namespace input
