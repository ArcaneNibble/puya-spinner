#include "registers.h"
#include "accel.h"
#include "led.h"


uint8_t debug_accel_whoami;
uint8_t debug_accel_ctrl_readback[6];
uint8_t debug_xxxxw[7];
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

    /// PB3 = accel interrupt
    GPIOB_MODER &= ~(0b11 << 6);
    EXTI_EXTICR1 = 0b01 << 24;
    EXTI_RTSR = 1 << 3;
    EXTI_IMR |= 1 << 3;

    // initial read to make sure interrupt pin is cleared
    while (!(GPIOB_IDR & (1 << 3))) {}
    accel_read_multi_reg(0x27, 7, debug_xxxxw);

    /// final interrupt enable
    NVIC_ISER = 1 << 6;
    
    while (1)
        asm volatile("wfi");
}

void EXTI2_3_IRQHandler() {
    static int i = 0;
    turn_on_led(i++);

    EXTI_PR = 1 << 3;
    accel_read_multi_reg(0x27, 7, debug_xxxxw);

    // this is faster, avoids divmod
    if (i == NUM_LEDS)
        i = 0;
}
