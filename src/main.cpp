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

#include "ws2812.h"
#include "Dmx.hpp"


#define NUM_LEDS 60

int main()
{

	Led leds[NUM_LEDS];

	//initPrintf();
	SysTick_Config(72000);
	initDmx();


	WS2812<NUM_LEDS> ws2812(leds, NUM_LEDS);
	ws2812.update();
	uint8_t h = 0;
	while(true)
	{

//		volatile uint8_t* dmxData = getDmxData();
//		for(int i = 0; i < NUM_LEDS; ++i)
//		{
//			leds[i].r = dmxData[3 * i + 1];
//			leds[i].g = dmxData[3 * i + 2];
//			leds[i].b = dmxData[3 * i + 3];
//		}


		for(int i = 0; i < NUM_LEDS; ++i)
		{
			leds[i].setHue(h);
		}
		++h;

		ws2812.update();

		Clock::delayMs(50);

	}


//	initPanTilt();
//
//	PelcoD left(1);
//	left.moveLeft(0xFF);
//	left.calcChecksum();
//
//	PelcoD right(1);
//	right.moveRight(0xFF);
//	right.calcChecksum();
//
//	PelcoD up(1);
//	up.moveUp(0xFF);
//	up.calcChecksum();
//
//	PelcoD down(1);
//	down.moveDown(0xFF);
//	down.calcChecksum();
//
//	const uint16_t numEntries = 12;
//	SequenceEntry seq[numEntries] = {
//			{&up, 200},
//			{&left, 800},
//			{&up, 200},
//			{&left, 800},
//			{&up, 200},
//			{&left, 800},
//			{&down, 200},
//			{&right, 800},
//			{&down, 200},
//			{&right, 800},
//			{&down, 200},
//			{&right, 800}
//
//	};

	//writeCommand(right);
	//Clock::delayMs(15000);
	//writeCommand(left);
	//Clock::delayMs(6000);



//	BlinkLed blinkLed;
//	ws2812Init();

	// Perform all necessary initialisations for the LED.
//	blinkLed.powerUp();

	// Infinite loop
	while (1)
	{
//		printf_("play\n");
//		playSequence(seq, numEntries);

		//USART1_Send("penis\n", 6);
//		printf_("%d\n", TIM_GetCounter(TIM4));
	}
  // Infinite loop, never return.
	return 0;
}

extern "C" {
void SysTick_Handler()
{
	Clock::tick(1);
//	if (TIM2->CCR4 == 0)
//	{
//		TIM2->CCR4 = 65;
//	}
//	else
//	{
//		TIM2->CCR4 = 0;
//	}
}
}

// ----------------------------------------------------------------------------
