#include "LSM303AGRAccel.h"
#include "CodalDmesg.h" // optional for debug prints

LSM303AGR_Accelerometer::LSM303AGR_Accelerometer(codal::I2C &i2c)
    : codal::Accelerometer(), _i2c(i2c) {}

void LSM303AGR_Accelerometer::init() {
    // CTRL_REG1_A (0x20): ODR and axis enable -> 0x57 (50Hz, XYZ enable) is common
    uint8_t buf[2];
    buf[0] = 0x20;
    buf[1] = 0x57;
    _i2c.write(LSM303AGR_ACC_ADDR, buf, 2);

    // CTRL_REG4 (0x23): block data update / high resolution if desired
    buf[0] = 0x23;
    buf[1] = 0x88; // BDU + high resolution (tweak as needed)
    _i2c.write(LSM303AGR_ACC_ADDR, buf, 2);

    // small delay
    fiber_sleep(20);
}

int16_t LSM303AGR_Accelerometer::read16(uint8_t reg) {
    uint8_t out[2] = {0, 0};
    // For multi-byte read we often set auto-increment bit (reg | 0x80) on many sensors;
    // LSM303AGR uses sequential registers; using reg with auto-increment might be ok.
    _i2c.write(LSM303AGR_ACC_ADDR, &reg, 1);
    _i2c.read(LSM303AGR_ACC_ADDR, out, 2);
    // registers are little-endian for OUT_X_L then OUT_X_H depending on datasheet.
    // Here we assume OUT_L, OUT_H -> low byte first.
    return (int16_t)(out[0] | (out[1] << 8));
}

int16_t LSM303AGR_Accelerometer::getX() {
    // OUT_X_L_A = 0x28
    return read16(0x28);
}

int16_t LSM303AGR_Accelerometer::getY() {
    // OUT_Y_L_A = 0x2A
    return read16(0x2A);
}

int16_t LSM303AGR_Accelerometer::getZ() {
    // OUT_Z_L_A = 0x2C
    return read16(0x2C);
}
