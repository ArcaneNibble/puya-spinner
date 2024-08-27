extern unsigned int _etext;
extern unsigned int _data;
extern unsigned int _edata;
extern unsigned int _bss;
extern unsigned int _ebss;
extern char _eusrstack;

extern void (*__preinit_array_start []) ();
extern void (*__preinit_array_end []) ();
extern void (*__init_array_start []) ();
extern void (*__init_array_end []) ();
extern void (*__fini_array_start []) ();
extern void (*__fini_array_end []) ();

extern void main();
void _start() {
    unsigned int *src, *dst;

    // copy initialized data
    src = &_etext;
    dst = &_data;
    while (dst < &_edata)
        *dst++ = *src++;
    
    // clear bss
    dst = &_bss;
    while (dst < &_ebss)
        *dst++ = 0;
    
    // init functions
    void (**fn)();

    fn = __preinit_array_start;
    while (fn != __preinit_array_end)
        (*fn++)();
    fn = __init_array_start;
    while (fn != __init_array_end)
        (*fn++)();

    // finally invoke main
    main();

    // fini functions
    fn = __fini_array_start;
    while (fn != __fini_array_end)
        (*fn++)();

    // hang
    while (1) {}
}

#define DUMMY __attribute__ ((weak, alias ("__irq_handler_dummy")))
void __irq_handler_dummy()
{
  	while (1);
}

DUMMY void NMI_Handler();
DUMMY void HardFault_Handler();
DUMMY void SVC_Handler();
DUMMY void PendSV_Handler();
DUMMY void SysTick_Handler();
DUMMY void WWDG_IRQHandler();
DUMMY void PVD_IRQHandler();
DUMMY void RTC_IRQHandler();
DUMMY void FLASH_IRQHandler();
DUMMY void RCC_IRQHandler();
DUMMY void EXTI0_1_IRQHandler();
DUMMY void EXTI2_3_IRQHandler();
DUMMY void EXTI4_15_IRQHandler();
DUMMY void DMA1_Channel1_IRQHandler();
DUMMY void DMA1_Channel2_3_IRQHandler();
DUMMY void ADC_COMP_IRQHandler();
DUMMY void TIM1_BRK_UP_TRG_COM_IRQHandler();
DUMMY void TIM1_CC_IRQHandler();
DUMMY void TIM3_IRQHandler();
DUMMY void LPTIM1_IRQHandler();
DUMMY void TIM14_IRQHandler();
DUMMY void TIM16_IRQHandler();
DUMMY void TIM17_IRQHandler();
DUMMY void I2C1_IRQHandler();
DUMMY void SPI1_IRQHandler();
DUMMY void SPI2_IRQHandler();
DUMMY void USART1_IRQHandler();
DUMMY void USART2_IRQHandler();
DUMMY void LED_IRQHandler();

__attribute__ ((used, section(".vectors")))
void (* const __vectors[])() =
{
    (void (*const)())&_eusrstack,
    _start,
    NMI_Handler,
    HardFault_Handler,

    0, 0, 0, 0,

    0, 0, 0,
    SVC_Handler,

    0, 0,
    PendSV_Handler,
    SysTick_Handler,

    WWDG_IRQHandler,
    PVD_IRQHandler,
    RTC_IRQHandler,
    FLASH_IRQHandler,

    RCC_IRQHandler,
    EXTI0_1_IRQHandler,
    EXTI2_3_IRQHandler,
    EXTI4_15_IRQHandler,

    0,
    DMA1_Channel1_IRQHandler,
    DMA1_Channel2_3_IRQHandler,
    0,

    ADC_COMP_IRQHandler,
    TIM1_BRK_UP_TRG_COM_IRQHandler,
    TIM1_CC_IRQHandler,
    0,

    TIM3_IRQHandler,
    LPTIM1_IRQHandler,
    0,
    TIM14_IRQHandler,

    0,
    TIM16_IRQHandler,
    TIM17_IRQHandler,
    I2C1_IRQHandler,

    0,
    SPI1_IRQHandler,
    SPI2_IRQHandler,
    USART1_IRQHandler,
    
    USART2_IRQHandler,
    0,
    LED_IRQHandler,
    0,
};
