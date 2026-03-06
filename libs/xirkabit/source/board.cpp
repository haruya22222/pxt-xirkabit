#include "pxt.h"
#include "LSM303AGRAccel.h"

using namespace codal;

// Use board's SDA/SCL defines; I2C_SDA0/I2C_SCL0 are common in PXT targets.
// Jika nama pin berbeda di board kamu, sesuaikan.
static codal::I2C i2c(I2C_SDA0, I2C_SCL0);
static LSM303AGR_Accelerometer acc(i2c);

// global pointer expected by many shims
codal::Accelerometer *accelerometer = &acc;

void board_init() {
    // pastikan inisialisasi lain juga dipanggil bila sudah ada
    acc.init();
}
