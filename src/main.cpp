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
#define FIXMATH_NO_CACHE

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
#define DMX_EFFECT_ID DMX_ADDRESS
#define DMX_SPEED DMX_ADDRESS + 1
#define DMX_BRIGHTNESS DMX_ADDRESS + 2
#define DMX_EFFECT_PARAM_1 DMX_ADDRESS + 3
#define DMX_EFFECT_PARAM_2 DMX_ADDRESS + 4
#define DMX_STROBE DMX_ADDRESS + 5

#define NUM_LEDS 60

uint8_t getDmxEffectId();
uint8_t getDmxSpeed();
uint8_t getDmxBrightness();
uint8_t getDmxEffectParam1();
uint8_t getDmxEffectParam2();
uint8_t getDmxstrobe();


int main()
{
	initPrintf();
	printf_("\n\nConartism LED Controller alive!\n\n");

	SysTick_Config(72000);
	initDmx();

	Led leds[NUM_LEDS];
	WS2812<NUM_LEDS> ws2812(leds, NUM_LEDS);

	EffectManager effectManager;
	effectManager.addEffect(updateColorFade);

	uint32_t lastTime = Clock::ticks;
	while(true)
	{
		if(Clock::ticks >= lastTime + 2)
		{//every 2 ms
			const uint8_t dt = Clock::ticks - lastTime;
			lastTime = Clock::ticks;

			const uint8_t effectId = getDmxEffectId();
			const uint8_t speed = getDmxSpeed();
			const uint8_t effectParam1 = getDmxEffectParam1();
			const uint8_t effectParam2 = getDmxEffectParam2();
			effectManager.execute(effectId, dt, speed, effectParam1, effectParam2, leds, NUM_LEDS);
			ws2812.update(getDmxBrightness());

			if(Clock::ticks - lastTime > 2)
			{
				printf_("Timing Error!: %d\n", Clock::ticks - lastTime);
			}
		}
	}
	return 0;
}

uint8_t getDmxSpeed()
{
	return getDmxData()[DMX_SPEED];
}


uint8_t getDmxEffectId()
{
	return getDmxData()[DMX_EFFECT_ID];
}

uint8_t getDmxBrightness()
{
	return getDmxData()[DMX_BRIGHTNESS];
}

uint8_t getDmxEffectParam1()
{
	return getDmxData()[DMX_EFFECT_PARAM_1];
}

uint8_t getDmxEffectParam2()
{
	return getDmxData()[DMX_EFFECT_PARAM_2];
}

uint8_t getDmxstrobe()
{
	return getDmxData()[DMX_STROBE];
}


