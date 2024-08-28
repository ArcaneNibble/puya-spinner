#pragma once
#include <stdint.h>

void i2c_init();
uint8_t accel_read_reg(uint8_t reg);
