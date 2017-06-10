/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdlib.h>
#include "printf.h"
#include "stm32f10x_tim.h"
#include "Clock.hpp"
#include "Led.hpp"
#include "EffectManager.hpp"
#include "Effects/ColorFadeEffect.hpp"
#include "ws2812.h"
#include "Dmx.hpp"

#define DMX_ADDRESS 1 //dmx addresses start with 1, not zero
#define NUM_LEDS 60

uint8_t getDmxEffectId();
uint8_t getDmxSpeed();


int main()
{
	initPrintf();
	printf_("Conartism LED Controller alive!");

	SysTick_Config(72000);
	initDmx();

	Led leds[NUM_LEDS];
	WS2812<NUM_LEDS> ws2812(leds, NUM_LEDS);

	EffectManager effectManager;
	effectManager.addEffect(updateColorFade);

	volatile uint8_t* dmxData = getDmxData();

	uint32_t lastTime = Clock::ticks;
	while(true)
	{
		if(Clock::ticks >= lastTime + 10)
		{//every 10 ms
			lastTime = Clock::ticks;
			const uint8_t effectId = getDmxEffectId();
			const uint8_t speed = getDmxSpeed();
			const uint8_t dt = 10; //FIXME if an effect takes longer than 10ms this is wrong
			effectManager.execute(effectId, dt, speed, leds, NUM_LEDS);
			ws2812.update();
		}
	}
	return 0;
}

uint8_t getDmxSpeed()
{
	return getDmxData()[DMX_ADDRESS + 1];
}


uint8_t getDmxEffectId()
{
	return getDmxData()[DMX_ADDRESS];
}



