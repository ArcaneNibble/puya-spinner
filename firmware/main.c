#include "registers.h"

static inline void set_hi(int pin) {
    GPIOA_BSRR = 1 << pin;
    GPIOA_MODER = (GPIOA_MODER & (~(3 << (pin * 2)))) | (1 << (pin * 2));
}

static inline void set_lo(int pin) {
    GPIOA_BSRR = 1 << (pin + 16);
    GPIOA_MODER = (GPIOA_MODER & (~(3 << (pin * 2)))) | (1 << (pin * 2));
}

static inline void set_tri(int pin) {
    GPIOA_MODER |= (3 << (pin * 2));
}

void main() {
    /// 24 MHz HSI
    RCC_ICSCR = RCC_ICSCR & 0xffff0000 | (4 << 13) | CAL_HSI_24M;

    /// GPIO setup
    RCC_IOPENR = 1;
    
    set_hi(0);
    set_lo(5);

    while (1) {
    }
}
