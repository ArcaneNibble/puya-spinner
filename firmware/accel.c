#include "accel.h"
#include "registers.h"
#include <stdatomic.h>
#include <string.h>

#define ACCEL_ADDR 0x18

#define CMD_FINISHED        0
#define CMD_READ_REG        1
#define CMD_WRITE_REG       2
static uint32_t i2c_cmd;
#define STEP_ST_SEND        0
#define STEP_ADDR_W_SEND    1
#define STEP_SR_SEND        2
#define STEP_ADDR_R_SEND    3
static uint32_t i2c_step;
static uint32_t i2c_reg;
static uint32_t i2c_len;
static uint32_t i2c_idx;
static uint8_t i2c_buffer[64];

void I2C1_IRQHandler() {
    // XXX this should be theoretically needed?
    // but it doesn't do anything afaict
    atomic_signal_fence(memory_order_acquire);
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
        // clear the bit
        I2C_SR2;
        if (i2c_step == STEP_ADDR_W_SEND) {
            I2C_DR = i2c_reg;
        } else {
            if (i2c_len == 1) {
                // nak, stop
                I2C_CR1 = (I2C_CR1 & ~(1 << 10)) | (1 << 9);
            }
        }
    }
    if (sr1 & (1 << 7)) {
        // TxE
        if (i2c_cmd == CMD_READ_REG) {
            i2c_step = STEP_SR_SEND;
            // start, ack
            I2C_CR1 |= (1 << 8) | (1 << 10);
        } else {
            I2C_DR = i2c_buffer[i2c_idx++];
            if (--i2c_len == 0) {
                // stop
                I2C_CR1 |= (1 << 9);
                i2c_cmd = CMD_FINISHED;
            }
        }
    }
    if (sr1 & (1 << 6)) {
        // RxNE
        i2c_buffer[i2c_idx++] = I2C_DR;
        i2c_len--;
        if (i2c_len == 1) {
            // in flight is the last byte, nak, stop
            I2C_CR1 = (I2C_CR1 & ~(1 << 10)) | (1 << 9);
        }
        if (i2c_len <= 1) {
            atomic_signal_fence(memory_order_release);
            i2c_cmd = CMD_FINISHED;
        }
    }
}

uint8_t accel_read_reg(uint8_t reg) {
    i2c_cmd = CMD_READ_REG;
    i2c_step = STEP_ST_SEND;
    i2c_reg = reg;
    i2c_len = 1;
    i2c_idx = 0;
    atomic_signal_fence(memory_order_release);
    // start
    I2C_CR1 |= (1 << 8);

    while (1) {
        if (i2c_cmd == CMD_FINISHED) {
            atomic_signal_fence(memory_order_acquire);
            return i2c_buffer[0];
        }
        asm volatile("wfi");
    }
}

void accel_read_multi_reg(uint8_t reg, uint32_t len, uint8_t *out) {
    i2c_cmd = CMD_READ_REG;
    i2c_step = STEP_ST_SEND;
    i2c_reg = reg;
    i2c_len = len;
    i2c_idx = 0;
    atomic_signal_fence(memory_order_release);
    // start
    I2C_CR1 |= (1 << 8);

    while (1) {
        if (i2c_cmd == CMD_FINISHED) {
            atomic_signal_fence(memory_order_acquire);
            memcpy(out, i2c_buffer, len);
            return;
        }
        asm volatile("wfi");
    }
}

void accel_write_reg(uint8_t reg, uint8_t val) {
    i2c_cmd = CMD_WRITE_REG;
    i2c_step = STEP_ST_SEND;
    i2c_reg = reg;
    i2c_len = 1;
    i2c_idx = 0;
    i2c_buffer[0] = val;
    atomic_signal_fence(memory_order_release);
    // start
    I2C_CR1 |= (1 << 8);

    while (1) {
        if (i2c_cmd == CMD_FINISHED) {
            return;
        }
        asm volatile("wfi");
    }
}
