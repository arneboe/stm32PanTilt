#pragma once
#include "stm32f10x.h"
#include <cstdint>

/** Interrupt driven dmx receiver on USART1.
 * Based on: http://www.emcu.it/STM32/Lighting-Control-using-DMX512-protocol-on-STM32/Lighting-Control-using-DMX512-protocol-on-STM32.html
 * Wiring:
 * Max485.rx -> PA10
 * Max485.rx -> PA1
 * Max485.rx -> PA0
 * */

#define DMX_CHANNEL_WIDTH 513

void initDmx();

/** Returned size = DMX_CHANNEL_WIDTH*/
volatile uint8_t* getDmxData();
