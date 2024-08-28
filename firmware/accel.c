#include "accel.h"
#include "registers.h"

// sigh, why does I2C never work right?

#define ACCEL_ADDR 0x18

uint8_t accel_read_reg(uint8_t reg) {
    uint16_t sr1;

    // start
    I2C_CR1 |= (1 << 8);
    while (!((sr1 = I2C_SR1) & (1 << 0))) {}

    // addr
    I2C_DR = ACCEL_ADDR << 1;
    while (!((sr1 = I2C_SR1) & (1 << 1))) {}
    I2C_SR2;

    // reg
    I2C_DR = reg;
    while (!((sr1 = I2C_SR1) & (1 << 2))) {}


    // start
    I2C_CR1 |= (1 << 8);
    while (!((sr1 = I2C_SR1) & (1 << 0))) {}

    // addr
    I2C_DR = (ACCEL_ADDR << 1) | 1;
    while (!((sr1 = I2C_SR1) & (1 << 1))) {}
    I2C_CR1 &= ~(1 << 10);
    I2C_SR2;

    // stop
    I2C_CR1 |= (1 << 9);

    // wait RxNE
    while (!((sr1 = I2C_SR1) & (1 << 6))) {}
    uint8_t b = I2C_DR;

    // wait full completion
    while (I2C_SR2 & (1 << 1)) {}
    return b;
}

void accel_read_multi_reg(uint8_t reg, uint32_t len, uint8_t *out) {
    uint16_t sr1;

    // start
    I2C_CR1 |= (1 << 8);
    while (!((sr1 = I2C_SR1) & (1 << 0))) {}

    // addr
    I2C_DR = ACCEL_ADDR << 1;
    while (!((sr1 = I2C_SR1) & (1 << 1))) {}
    I2C_SR2;

    // reg
    I2C_DR = reg | 0x80;    // WTF!
    while (!((sr1 = I2C_SR1) & (1 << 2))) {}


    // start
    I2C_CR1 |= (1 << 8);
    while (!((sr1 = I2C_SR1) & (1 << 0))) {}

    // addr
    I2C_DR = (ACCEL_ADDR << 1) | 1;
    while (!((sr1 = I2C_SR1) & (1 << 1))) {}
    // ack
    I2C_CR1 |= (1 << 10);
    I2C_SR2;

    uint32_t idx = 0;
    while (len > 3) {
        // wait for byte
        while (!((sr1 = I2C_SR1) & (1 << 6))) {}
        out[idx++] = I2C_DR;
        len--;
    }

    // wait for BTF hack thing
    while (!((sr1 = I2C_SR1) & (1 << 2))) {}
    // nack
    I2C_CR1 &= ~(1 << 10);
    out[idx++] = I2C_DR;
    while (!((sr1 = I2C_SR1) & (1 << 2))) {}
    // stop
    I2C_CR1 |= (1 << 9);
    out[idx++] = I2C_DR;
    out[idx++] = I2C_DR;

    // wait full completion
    while (I2C_SR2 & (1 << 1)) {}
}

void accel_write_reg(uint8_t reg, uint8_t val) {
    uint16_t sr1;

    // start
    I2C_CR1 |= (1 << 8);
    while (!((sr1 = I2C_SR1) & (1 << 0))) {}

    // addr
    I2C_DR = ACCEL_ADDR << 1;
    while (!((sr1 = I2C_SR1) & (1 << 1))) {}
    I2C_SR2;

    // reg
    I2C_DR = reg;
    // val
    I2C_DR = val;

    // BTF
    while (!((sr1 = I2C_SR1) & (1 << 2))) {}

    // stop
    I2C_CR1 |= (1 << 9);
    while (I2C_SR2 & (1 << 1)) {}
}
