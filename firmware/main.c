#include "registers.h"
#include "accel.h"

static inline void set_hi(int pin) {
    GPIOA_BSRR = 1 << pin;
    GPIOA_MODER &= ~(0b10 << (pin * 2));
}

static inline void set_lo(int pin) {
    GPIOA_BSRR = 1 << (pin + 16);
    GPIOA_MODER &= ~(0b10 << (pin * 2));
}

#define NUM_LEDS 30
const uint8_t led_pins[NUM_LEDS * 2] = {
    5, 0,
    0, 5,
    5, 2,
    2, 5,
    4, 5,

    5, 4,
    5, 3,
    3, 5,
    5, 1,
    1, 5,

    3, 1,
    1, 3,
    2, 1,
    1, 2,
    4, 1,

    1, 4,
    0, 1,
    1, 0,
    0, 2,
    2, 0,

    0, 3,
    3, 0,
    4, 0,
    0, 4,
    3, 4,

    4, 3,
    4, 2,
    2, 4,
    2, 3,
    3, 2,
};
void turn_on_led(int led) {
    // make everything (relevant) tristate
    GPIOA_MODER |= 0xfff;
    // turn on *just* this LED
    set_hi(led_pins[led * 2 + 0]);
    set_lo(led_pins[led * 2 + 1]);
}


uint8_t debug_accel_whoami;
void main() {
    /// 24 MHz HSI
    RCC_ICSCR = RCC_ICSCR & 0xffff0000 | (4 << 13) | CAL_HSI_24M;

    /// GPIO setup
    RCC_IOPENR = 0b100001;

    /// I2C setup
    RCC_APBENR1 |= (1 << 21);
    // 24 MHz system clock, interrupts enable
    I2C_CR2 = (0b111 << 8) | 24;
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
    // interrupt
    NVIC_ISER = 1 << 23;

    debug_accel_whoami = accel_read_reg(0x0f);
    
    int i = 0;
    while (1) {
        turn_on_led(i++);
        for (int j = 0; j < 500000; j++) asm volatile("");
        // this is faster, avoids divmod
        if (i == NUM_LEDS)
            i = 0;
    }
}
