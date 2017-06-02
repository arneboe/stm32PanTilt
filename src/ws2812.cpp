
// ST lib includes
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"
#include "ws2812.h"
#include "printf.h"


//#define TIM1_CCR1_Address 0x40012C34	// physical memory address of Timer 3 CCR1 register

static TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
static TIM_OCInitTypeDef TIM_OCInitStructure;
static GPIO_InitTypeDef GPIO_InitStructure;

#define TIMING_ONE  75
#define TIMING_ZERO 29
#define NUM_LEDS 2
#define BUFFER_SIZE (NUM_LEDS*24 + 40) //40 low cycles in the end to signal reset (50us)

uint8_t buffer[BUFFER_SIZE];

struct Led
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}__attribute__ ((__packed__));

Led ledBuffer[NUM_LEDS];

void ws2812Init(void)
{
	printf_("init ws2812\n");
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
	TIM_OCInitStructure.TIM_Pulse = 65;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);

	//TIM_SetCompare4(TIM2, 25); //25 = 0
	TIM_SetCompare4(TIM2, 65); //65 = 1
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);


	//init dma
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitTypeDef dmaInit;

	DMA_StructInit(&dmaInit);

	dmaInit.DMA_PeripheralBaseAddr = &TIM2->CCR4;//write to tim2 capture compare 4
    dmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //CCR4 is uint16_t
    dmaInit.DMA_MemoryBaseAddr = buffer;
    dmaInit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dmaInit.DMA_MemoryInc = DMA_MemoryInc_Enable; //increase memory read address after each update
    dmaInit.DMA_BufferSize = BUFFER_SIZE;
    dmaInit.DMA_DIR = DMA_DIR_PeripheralDST;
    dmaInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dmaInit.DMA_Mode = DMA_Mode_Circular;
    dmaInit.DMA_Priority = DMA_Priority_High;
    dmaInit.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA1_Channel1, dmaInit);

//	/* configure DMA */

//	/* DMA clock enable */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
//
//	/* DMA1 Channel2 Config */
//	DMA_DeInit(DMA1_Stream5);
//
//	// USART TX DMA Channel Config
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM2->CCR2;
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)led_dma.buffer;    // this is the buffer memory
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//	DMA_InitStructure.DMA_BufferSize = 0;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
//	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
//	DMA_Init(DMA1_Stream5, &DMA_InitStructure);
//
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_LOW_PRI;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);
//	DMA_ITConfig(DMA1_Stream5, DMA_IT_HT, ENABLE);
//
//	/* TIM2 CC2 DMA Request enable */
//	TIM_DMACmd(TIM2, TIM_DMA_CC2, ENABLE);


}
//
//static void fillLed(uint16_t *buffer, uint8_t *color)
//{
//    int i;
//
//    for(i=0; i<8; i++) // GREEN data
//	{
//	    buffer[i] = ((color[1]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
//	}
//	for(i=0; i<8; i++) // RED
//	{
//	    buffer[8+i] = ((color[0]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
//	}
//	for(i=0; i<8; i++) // BLUE
//	{
//	    buffer[16+i] = ((color[2]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
//	}
//}
//
//static int current_led = 0;
//static int total_led = 0;
//static uint8_t (*color_led)[3] = NULL;
//
//void ws2812Send(uint8_t (*color)[3], uint16_t len)
//{
//    int i;
//	if(len<1) return;
//
//	//Wait for previous transfer to be finished
//	xSemaphoreTake(allLedDone, portMAX_DELAY);
//
//	// Set interrupt context ...
//	current_led = 0;
//	total_led = len;
//	color_led = color;
//
//    for(i=0; (i<LED_PER_HALF) && (current_led<total_led+2); i++, current_led++) {
//        if (current_led<total_led)
//            fillLed(led_dma.begin+(24*i), color_led[current_led]);
//        else
//            bzero(led_dma.begin+(24*i), sizeof(led_dma.begin));
//    }
//
//    for(i=0; (i<LED_PER_HALF) && (current_led<total_led+2); i++, current_led++) {
//        if (current_led<total_led)
//            fillLed(led_dma.end+(24*i), color_led[current_led]);
//        else
//            bzero(led_dma.end+(24*i), sizeof(led_dma.end));
//    }
//
//	DMA1_Stream5->NDTR = sizeof(led_dma.buffer) / sizeof(led_dma.buffer[0]); // load number of bytes to be transferred
//	DMA_Cmd(DMA1_Stream5, ENABLE); 			// enable DMA channel 2
//	TIM_Cmd(TIM2, ENABLE);                      // Go!!!
//}
//
//void ws2812DmaIsr(void)
//{
//    portBASE_TYPE xHigherPriorityTaskWoken;
//    uint16_t * buffer;
//    int i;
//
//    if (total_led == 0)
//    {
//      TIM_Cmd(TIM2, DISABLE);
//    	DMA_Cmd(DMA1_Stream5, DISABLE);
//    }
//
//    if (DMA_GetITStatus(DMA1_Stream5, DMA_IT_HTIF5))
//    {
//      DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_HTIF5);
//      buffer = led_dma.begin;
//    }
//
//    if (DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
//    {
//      DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
//      buffer = led_dma.end;
//    }
//
//    for(i=0; (i<LED_PER_HALF) && (current_led<total_led+2); i++, current_led++) {
//      if (current_led<total_led)
//          fillLed(buffer+(24*i), color_led[current_led]);
//      else
//          bzero(buffer+(24*i), sizeof(led_dma.end));
//    }
//
//    if (current_led >= total_led+2) {
//      xSemaphoreGiveFromISR(allLedDone, &xHigherPriorityTaskWoken);
//
//	    TIM_Cmd(TIM2, DISABLE); 					// disable Timer 3
//	    DMA_Cmd(DMA1_Stream5, DISABLE); 			// disable DMA stream4
//
//	    total_led = 0;
//    }
//}
