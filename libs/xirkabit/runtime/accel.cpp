#include "pxt.h"

using namespace pxt;

// Automatically provided by CODAL for your board
extern codal::Accelerometer *accelerometer;

namespace input {

int accelX() {
    return accelerometer->getX();
}

int accelY() {
    return accelerometer->getY();
}

int accelZ() {
    return accelerometer->getZ();
}

int accelStrength() {
    int ax = accelX();
    int ay = accelY();
    int az = accelZ();
    return (int)sqrt(ax * ax + ay * ay + az * az);
}

} // namespace input
