#include "registers.h"
#include "accel.h"
#include "led.h"
#include <math.h>

uint8_t debug_accel_whoami;
uint8_t debug_accel_ctrl_readback[6];
#define DEBUG_BUF_NENTS     1200
int16_t debug_accel_buffer[DEBUG_BUF_NENTS];
uint32_t debug_accel_buffer_idx;

void main() {
    /// 24 MHz HSI
    RCC_ICSCR = RCC_ICSCR & 0xffff0000 | (4 << 13) | CAL_HSI_24M;

    /// GPIO power on
    RCC_IOPENR = 0b100011;

    /// I2C setup
    RCC_APBENR1 |= (1 << 21);
    // 24 MHz system clock
    I2C_CR2 = 24;
    // 400 kHz I2C
    I2C_CCR = (1 << 15) | 20;
    I2C_TRISE = 8;
    // enable
    I2C_CR1 = 1;
    // gpio
    GPIOF_OTYPER = 0b11;
    GPIOF_OSPEEDR = 0b1010;
    GPIOF_AFRL = 0xcc;
    GPIOF_MODER = (GPIOF_MODER & ~0b1111) | 0b1010;

    /// Accel setup
    debug_accel_whoami = accel_read_reg(0x0f);
    accel_write_reg(0x20, 0b01110111);
    accel_write_reg(0x21, 0b00000000);
    accel_write_reg(0x22, 0b00010000);
    accel_write_reg(0x23, 0b00001000);
    accel_write_reg(0x24, 0b00000000);
    accel_write_reg(0x25, 0b00000000);
    accel_read_multi_reg(0x20, 6, debug_accel_ctrl_readback);

    /// xxx dummy data
    for (int i = 0; i < NUM_SECTORS; i++) {
        led_data[i] = (1 << (i % (NUM_LEDS + 1))) - 1;
    }

    /// systick setup, 24 kHz = 30 LEDs * 400 Hz * 2x doubling
    SYST_CVR = 0;
    SYST_RVR = 999;
    SYST_CSR = 0b111;

    /// PB3 = accel interrupt
    GPIOB_MODER &= ~(0b11 << 6);
    EXTI_EXTICR1 = 0b01 << 24;
    EXTI_RTSR = 1 << 3;
    EXTI_IMR |= 1 << 3;
    NVIC_IPR1 = 0xc0 << 16;     // lower prio than led

    // initial read to make sure interrupt pin is cleared
    while (!(GPIOB_IDR & (1 << 3))) {}
    uint8_t dummy_accel_buf[7];
    accel_read_multi_reg(0x27, 7, dummy_accel_buf);

    /// final interrupt enable
    NVIC_ISER = 1 << 6;
    
    while (1)
        asm volatile("wfi");
}

void EXTI2_3_IRQHandler() {
    EXTI_PR = 1 << 3;
    uint8_t accel_buf[7];
    accel_read_multi_reg(0x27, 7, accel_buf);

    int16_t x = accel_buf[1] | (accel_buf[2] << 8);
    int16_t y = accel_buf[3] | (accel_buf[4] << 8);
    int16_t z = accel_buf[5] | (accel_buf[6] << 8);

    debug_accel_buffer[debug_accel_buffer_idx++] = x;
    debug_accel_buffer[debug_accel_buffer_idx++] = y;
    debug_accel_buffer[debug_accel_buffer_idx++] = z;
    if (debug_accel_buffer_idx == DEBUG_BUF_NENTS)
        debug_accel_buffer_idx = 0;

    // maths!
    float angle = atan2f(-y, x);
    float led_sector_f = angle * ((float)(NUM_SECTORS / 2) / (float)(M_PI));
    int led_sector = floorf(led_sector_f);
    if (led_sector < 0)
        led_sector += NUM_SECTORS;
    led_current_sector = led_sector;
}
