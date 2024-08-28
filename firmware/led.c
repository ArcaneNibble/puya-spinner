#include "led.h"
#include "registers.h"

uint32_t led_data[NUM_SECTORS];
int led_current_sector;

static inline void set_hi(int pin) {
    GPIOA_BSRR = 1 << pin;
    GPIOA_MODER &= ~(0b10 << (pin * 2));
}

static inline void set_lo(int pin) {
    GPIOA_BSRR = 1 << (pin + 16);
    GPIOA_MODER &= ~(0b10 << (pin * 2));
}

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
static inline void turn_on_led(int led) {
    // turn on *just* this LED
    set_hi(led_pins[led * 2 + 0]);
    set_lo(led_pins[led * 2 + 1]);
}

uint32_t current_led;
void SysTick_Handler() {
    uint32_t current_word = led_data[led_current_sector];

    // turn off leds
    GPIOA_MODER |= 0xfff;

    if (current_word & (1 << current_led))
        turn_on_led(current_led);

    current_led++;
    if (current_led == NUM_LEDS) {
        current_led = 0;

        // // test
        // led_current_sector++;
        // if (led_current_sector == NUM_SECTORS)
        //     led_current_sector = 0;
    }
}
