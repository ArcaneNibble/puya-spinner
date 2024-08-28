#pragma once
#include <stdint.h>

#define NUM_LEDS 30

#define NUM_SECTORS 128
extern uint32_t led_data[NUM_SECTORS];
extern int led_current_sector;
