#pragma once

#include "CodalConfig.h"
#include "Accelerometer.h"
#include "I2C.h"

#define LSM303AGR_ACC_ADDR 0x19

class LSM303AGR_Accelerometer : public codal::Accelerometer {
  public:
    LSM303AGR_Accelerometer(codal::I2C &i2c);
    virtual void init();
    virtual int16_t getX();
    virtual int16_t getY();
    virtual int16_t getZ();

  private:
    codal::I2C &_i2c;
    int16_t read16(uint8_t reg);
};
