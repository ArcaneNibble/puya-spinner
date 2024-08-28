#include "accel.h"
#include "registers.h"
#include <stdatomic.h>

#define ACCEL_ADDR 0x18

#define CMD_FINISHED        0
#define CMD_READ_ONE_REG    1
static uint8_t i2c_cmd;
#define STEP_ST_SEND        0
#define STEP_ADDR_W_SEND    1
#define STEP_SR_SEND        2
#define STEP_ADDR_R_SEND    3
static uint8_t i2c_step;
static uint8_t i2c_read_one_reg;
static uint8_t i2c_read_one_result;

void I2C1_IRQHandler() {
    uint16_t sr1 = I2C_SR1;

    if (sr1 & (1 << 0)) {
        // SB
        if (i2c_step == STEP_ST_SEND) {
            i2c_step = STEP_ADDR_W_SEND;
            I2C_DR = ACCEL_ADDR << 1;
        } else {
            i2c_step = STEP_ADDR_R_SEND;
            I2C_DR = (ACCEL_ADDR << 1) | 1;
        }
    }
    if (sr1 & (1 << 1)) {
        // ADDR
        if (i2c_step == STEP_ADDR_W_SEND) {
            // todo sending more bytes
            I2C_SR2;    // clear ADDR
            I2C_DR = i2c_read_one_reg;
        } else {
            // todo read more bytes
            // nak
            I2C_CR1 &= ~(1 << 10);
            // clear addr
            I2C_SR2;
            // stop
            I2C_CR1 |= (1 << 9);
        }
    }
    if (sr1 & (1 << 2)) {
        // BTF
        // todo other sequencing
        i2c_step = STEP_SR_SEND;
        // start
        I2C_CR1 |= (1 << 8);
    }
    if (sr1 & (1 << 6)) {
        // RxNE
        // todo read more bytes
        i2c_read_one_result = I2C_DR;
        atomic_signal_fence(memory_order_release);
        i2c_cmd = CMD_FINISHED;
    }
}

uint8_t accel_read_reg(uint8_t reg) {
    i2c_cmd = CMD_READ_ONE_REG;
    i2c_step = STEP_ST_SEND;
    i2c_read_one_reg = reg;
    atomic_signal_fence(memory_order_release);
    // start
    I2C_CR1 |= (1 << 8);

    while (1) {
        if (i2c_cmd == CMD_FINISHED) {
            atomic_signal_fence(memory_order_acquire);
            return i2c_read_one_result;
        }
        asm volatile("wfi");
    }

    // uint16_t sr1;

    // // start
    // I2C_CR1 |= (1 << 8);
    // while (!((sr1 = I2C_SR1) & (1 << 0))) {}

    // // addr
    // I2C_DR = ACCEL_ADDR << 1;
    // while (!((sr1 = I2C_SR1) & (1 << 1))) {}
    // I2C_SR2;

    // // reg
    // I2C_DR = reg;
    // while (!((sr1 = I2C_SR1) & (1 << 2))) {}


    // // start
    // I2C_CR1 |= (1 << 8);
    // while (!((sr1 = I2C_SR1) & (1 << 0))) {}

    // // addr
    // I2C_DR = (ACCEL_ADDR << 1) | 1;
    // while (!((sr1 = I2C_SR1) & (1 << 1))) {}
    // I2C_CR1 &= ~(1 << 10);
    // I2C_SR2;

    // // stop
    // I2C_CR1 |= (1 << 9);
    // while (!((sr1 = I2C_SR1) & (1 << 6))) {}
    // return I2C_DR;
}
