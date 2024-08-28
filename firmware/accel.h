#pragma once
#include <stdint.h>

void i2c_init();
uint8_t accel_read_reg(uint8_t reg);
void accel_read_multi_reg(uint8_t reg, uint8_t len, uint8_t *out);
