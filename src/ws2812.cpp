// ST lib includes
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"
#include "ws2812.h"

#define LOW_PULSE 25
#define HIGH_PULSE 65

#define CHECK_BIT(var,i) ((var) & (1<<(i)))


void copyLed(const Led& led, uint8_t* buf)
{
    for(int i = 0; i < 8; i++) // GREEN data
	{
	    buf[i] = ((led.r << i) & 0x80)? HIGH_PULSE : LOW_PULSE;
	}
    for(int i = 0; i < 8; i++) // GREEN data
	{
	    buf[i+8] = ((led.g << i) & 0x80)? HIGH_PULSE : LOW_PULSE;
	}
    for(int i = 0; i < 8; i++) // GREEN data
	{
	    buf[i+16] = ((led.b << i) & 0x80)? HIGH_PULSE : LOW_PULSE;
	}
}

