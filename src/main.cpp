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
#include "PanTiltCtrl.hpp"
#include "pelcoD/PelcoD.hpp"
#include "Clock.hpp"

#include "ws2812.h"
#include "Dmx.hpp"


// ----------------------------------------------------------------------------
//
// Standalone STM32F1 led blink sample (trace via NONE).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// Then demonstrates how to blink a led with 1 Hz, using a
// continuous loop and SysTick delays.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//
// The external clock frequency is specified as a preprocessor definition
// passed to the compiler via a command line option (see the 'C/C++ General' ->
// 'Paths and Symbols' -> the 'Symbols' tab, if you want to change it).
// The value selected during project creation was HSE_VALUE=8000000.
//
// Note: The default clock settings take the user defined HSE_VALUE and try
// to reach the maximum possible system clock. For the default 8 MHz input
// the result is guaranteed, but for other values it might not be possible,
// so please adjust the PLL settings in system/src/cmsis/system_stm32f10x.c
//





// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.



int main()
{
//init_UART1(); //init debug usart port
	SysTick_Config(72000);

	dmxInit();

	while(true)
	{
		Clock::delayMs(1000);

		for(int i = 0; i < 20; ++i)
		{
			printf_("%i ", vu8_DMX_Buff[i]);
		}
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
