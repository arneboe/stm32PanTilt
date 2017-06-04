#pragma once
#include <stdint.h>
#include <cstring> //for size_t
#include "printf.h"

struct Led
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}__attribute__ ((__packed__));


template <uint16_t NUM_LEDS>
class WS2812
{
public:
	WS2812(const Led* leds, size_t num);

	/**Update internal buffer from led data
	 * Data from internal buffer will be send repeatedly until update is called again */
	void update();
private:

	void init();
	void copyLed(const Led& led, uint8_t* buf);

	/**The dma buffer */
	static const size_t bufferSize = NUM_LEDS * 24 + 40;
	uint8_t buffer[bufferSize]; //+40 for 50us reset signal
	/**Led buffer used by user for drawing*/
	const Led* leds;
	const size_t numLeds;
};

template <uint16_t NUM_LEDS>
WS2812<NUM_LEDS>::WS2812(const Led* _leds, size_t num) : leds(_leds), numLeds(num)
{
	printf_("init ws2812\n");

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 89;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0; //off initially
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);

	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);


	//init dma
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitTypeDef dmaInit;

	DMA_StructInit(&dmaInit);

	dmaInit.DMA_PeripheralBaseAddr = (uint32_t)&(TIM2->CCR4);//write to tim2 capture compare 4
    dmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //CCR4 is uint16_t
    dmaInit.DMA_MemoryBaseAddr = (uint32_t)buffer;
    dmaInit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dmaInit.DMA_MemoryInc = DMA_MemoryInc_Enable; //increase memory read address after each update
    dmaInit.DMA_BufferSize = bufferSize;
    dmaInit.DMA_DIR = DMA_DIR_PeripheralDST;
    dmaInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dmaInit.DMA_Mode = DMA_Mode_Circular;
    dmaInit.DMA_Priority = DMA_Priority_High;
    dmaInit.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA1_Channel2, &dmaInit);
    TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);
    DMA_Cmd(DMA1_Channel2, ENABLE);

}
