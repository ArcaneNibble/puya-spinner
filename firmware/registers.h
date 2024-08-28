#include <stdint.h>

#define CAL_HSI_4M      (*(const uint32_t*)0x1fff0f00)
#define CAL_HSI_8M      (*(const uint32_t*)0x1fff0f04)
#define CAL_HSI_16M     (*(const uint32_t*)0x1fff0f08)
#define CAL_HSI_22_12M  (*(const uint32_t*)0x1fff0f0c)
#define CAL_HSI_24M     (*(const uint32_t*)0x1fff0f10)

#define I2C_CR1         (*(volatile uint16_t*)0x40005400)
#define I2C_CR2         (*(volatile uint16_t*)0x40005404)
#define I2C_OAR1        (*(volatile uint16_t*)0x40005408)
#define I2C_DR          (*(volatile uint16_t*)0x40005410)
#define I2C_SR1         (*(volatile uint16_t*)0x40005414)
#define I2C_SR2         (*(volatile uint16_t*)0x40005418)
#define I2C_CCR         (*(volatile uint16_t*)0x4000541c)
#define I2C_TRISE       (*(volatile uint16_t*)0x40005420)

#define TIM1_CR1        (*(volatile uint32_t*)0x40012c00)
#define TIM1_CR2        (*(volatile uint32_t*)0x40012c04)
#define TIM1_SMCR       (*(volatile uint32_t*)0x40012c08)
#define TIM1_DIER       (*(volatile uint32_t*)0x40012c0c)
#define TIM1_SR         (*(volatile uint32_t*)0x40012c10)
#define TIM1_EGR        (*(volatile uint32_t*)0x40012c14)
#define TIM1_CCMR1      (*(volatile uint32_t*)0x40012c18)
#define TIM1_CCMR2      (*(volatile uint32_t*)0x40012c1c)
#define TIM1_CCER       (*(volatile uint32_t*)0x40012c20)
#define TIM1_CNT        (*(volatile uint32_t*)0x40012c24)
#define TIM1_PSC        (*(volatile uint32_t*)0x40012c28)
#define TIM1_ARR        (*(volatile uint32_t*)0x40012c2c)
#define TIM1_RCR        (*(volatile uint32_t*)0x40012c30)
#define TIM1_CCR1       (*(volatile uint32_t*)0x40012c34)
#define TIM1_CCR2       (*(volatile uint32_t*)0x40012c38)
#define TIM1_CCR3       (*(volatile uint32_t*)0x40012c3c)
#define TIM1_CCR4       (*(volatile uint32_t*)0x40012c40)
#define TIM1_BDTR       (*(volatile uint32_t*)0x40012c44)
#define TIM1_DCR        (*(volatile uint32_t*)0x40012c48)
#define TIM1_DMAR       (*(volatile uint32_t*)0x40012c4c)

#define RCC_CR          (*(volatile uint32_t*)0x40021000)
#define RCC_ICSCR       (*(volatile uint32_t*)0x40021004)
#define RCC_CFGR        (*(volatile uint32_t*)0x40021008)
#define RCC_PLLCFGR     (*(volatile uint32_t*)0x4002100c)
#define RCC_IOPENR      (*(volatile uint32_t*)0x40021034)
#define RCC_AHBENR      (*(volatile uint32_t*)0x40021038)
#define RCC_APBENR1     (*(volatile uint32_t*)0x4002103c)
#define RCC_APBENR2     (*(volatile uint32_t*)0x40021040)

#define GPIOA_MODER     (*(volatile uint32_t*)0x50000000)
#define GPIOA_OTYPER    (*(volatile uint32_t*)0x50000004)
#define GPIOA_OSPEEDR   (*(volatile uint32_t*)0x50000008)
#define GPIOA_PUPDR     (*(volatile uint32_t*)0x5000000c)
#define GPIOA_IDR       (*(volatile uint32_t*)0x50000010)
#define GPIOA_ODR       (*(volatile uint32_t*)0x50000014)
#define GPIOA_BSRR      (*(volatile uint32_t*)0x50000018)
#define GPIOA_LCKR      (*(volatile uint32_t*)0x5000001c)
#define GPIOA_AFRL      (*(volatile uint32_t*)0x50000020)
#define GPIOA_AFRH      (*(volatile uint32_t*)0x50000024)
#define GPIOA_BRR       (*(volatile uint32_t*)0x50000028)

#define GPIOB_MODER     (*(volatile uint32_t*)0x50000400)
#define GPIOB_OTYPER    (*(volatile uint32_t*)0x50000404)
#define GPIOB_OSPEEDR   (*(volatile uint32_t*)0x50000408)
#define GPIOB_PUPDR     (*(volatile uint32_t*)0x5000040c)
#define GPIOB_IDR       (*(volatile uint32_t*)0x50000410)
#define GPIOB_ODR       (*(volatile uint32_t*)0x50000414)
#define GPIOB_BSRR      (*(volatile uint32_t*)0x50000418)
#define GPIOB_LCKR      (*(volatile uint32_t*)0x5000041c)
#define GPIOB_AFRL      (*(volatile uint32_t*)0x50000420)
#define GPIOB_AFRH      (*(volatile uint32_t*)0x50000424)
#define GPIOB_BRR       (*(volatile uint32_t*)0x50000428)

#define GPIOF_MODER     (*(volatile uint32_t*)0x50001400)
#define GPIOF_OTYPER    (*(volatile uint32_t*)0x50001404)
#define GPIOF_OSPEEDR   (*(volatile uint32_t*)0x50001408)
#define GPIOF_PUPDR     (*(volatile uint32_t*)0x5000140c)
#define GPIOF_IDR       (*(volatile uint32_t*)0x50001410)
#define GPIOF_ODR       (*(volatile uint32_t*)0x50001414)
#define GPIOF_BSRR      (*(volatile uint32_t*)0x50001418)
#define GPIOF_LCKR      (*(volatile uint32_t*)0x5000141c)
#define GPIOF_AFRL      (*(volatile uint32_t*)0x50001420)
#define GPIOF_AFRH      (*(volatile uint32_t*)0x50001424)
#define GPIOF_BRR       (*(volatile uint32_t*)0x50001428)

#define NVIC_ISER       (*(volatile uint32_t*)0xe000e100)
#define NVIC_ICER       (*(volatile uint32_t*)0xe000e180)
#define NVIC_ISPR       (*(volatile uint32_t*)0xe000e200)
#define NVIC_ICPR       (*(volatile uint32_t*)0xe000e280)
#define NVIC_IPR0       (*(volatile uint32_t*)0xe000e400)
#define NVIC_IPR1       (*(volatile uint32_t*)0xe000e404)
#define NVIC_IPR2       (*(volatile uint32_t*)0xe000e408)
#define NVIC_IPR3       (*(volatile uint32_t*)0xe000e40c)
#define NVIC_IPR4       (*(volatile uint32_t*)0xe000e410)
#define NVIC_IPR5       (*(volatile uint32_t*)0xe000e414)
#define NVIC_IPR6       (*(volatile uint32_t*)0xe000e418)
#define NVIC_IPR7       (*(volatile uint32_t*)0xe000e41c)
