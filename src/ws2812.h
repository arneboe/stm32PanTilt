#pragma once
#include <stdint.h>
#include <cstring> //for size_t
#include "Led.hpp"


template <uint16_t NUM_LEDS>
class WS2812
{
public:
	WS2812(const Led* leds, int16_t* mapping, size_t num);

	/**Update internal buffer from led data
	 * Data from internal buffer will be send repeatedly until update is called again */
	void update(uint8_t brightness);
private:

	void copyLed(const Led& led, uint8_t* buf);

	/**The dma buffer */
	static const size_t bufferSize = NUM_LEDS * 24 + 40;
	uint8_t buffer[bufferSize] = {0}; //+40 for 50us reset signal
	/**Led buffer used by user for drawing*/
	const Led* leds;
	const int16_t* mapping;
	const size_t numLeds;
};

template <uint16_t NUM_LEDS>
WS2812<NUM_LEDS>::WS2812(const Led* _leds, int16_t* mapping, size_t num) : leds(_leds), mapping(mapping), numLeds(num)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 89;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);
	//TIM_CtrlPWMOutputs(TIM3, ENABLE); //not needed for TIM3
	TIM_Cmd(TIM3, ENABLE);


	//init dma
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitTypeDef dmaInit;

	DMA_StructInit(&dmaInit);

	dmaInit.DMA_PeripheralBaseAddr = (uint32_t)&(TIM3->CCR2);//write to TIM3 capture compare 2
	dmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //CCR2 is uint16_t
	dmaInit.DMA_MemoryBaseAddr = (uint32_t)buffer;
	dmaInit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dmaInit.DMA_MemoryInc = DMA_MemoryInc_Enable; //increase memory read address after each update
	dmaInit.DMA_BufferSize = bufferSize;
	dmaInit.DMA_DIR = DMA_DIR_PeripheralDST;
	dmaInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dmaInit.DMA_Mode = DMA_Mode_Circular;
	dmaInit.DMA_Priority = DMA_Priority_High;
	dmaInit.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel3, &dmaInit);
	TIM_DMACmd(TIM3, TIM_DMA_Update, ENABLE);
	DMA_Cmd(DMA1_Channel3, ENABLE);
}

template <uint16_t NUM_LEDS>
void WS2812<NUM_LEDS>::update(uint8_t brightness)
{
	Fix16 fixBrightness(brightness);
	fixBrightness /= (int16_t)255;
	for(int i = 0; i < NUM_LEDS; ++i)
	{
		const int mappedI = mapping[i];
		if(-1 == mappedI)
			continue;
		Led l(leds[mappedI]);
		l.setBrightness(fixBrightness);
		// (* 24) because every led takes up 24 bytes in the buffer
		copyLed(l, buffer + i * 24);
	}

	//NOTE the last 40 bytes in buffer are never touched.
	//     They are zeroed in the constructor and always remain zero.
	//     They cause a 50us low output which signals reset to the leds
}

template <uint16_t NUM_LEDS>
void WS2812<NUM_LEDS>::copyLed(const Led& led, uint8_t* buf)
{
	static const uint8_t low = 25;
	static const uint8_t high = 65;

    for(int i = 0; i < 8; i++) // GREEN data
	{
	    buf[i] = ((led.r << i) & 0x80)? high : low;
	}
    for(int i = 0; i < 8; i++) // GREEN data
	{
	    buf[i+8] = ((led.g << i) & 0x80)? high : low;
	}
    for(int i = 0; i < 8; i++) // GREEN data
	{
	    buf[i+16] = ((led.b << i) & 0x80)? high : low;
	}
}


