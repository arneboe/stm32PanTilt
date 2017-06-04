#include "PanTiltCtrl.hpp"
#include "stm32f10x.h"
#include "Clock.hpp"

void initPanTilt()
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;


    // USART1 TX-Pin initialize
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // USART and clock initialize
    USART_ClockInitTypeDef USART_ClockInitStructure;
    USART_ClockStructInit(&USART_ClockInitStructure);
    USART_ClockInit(USART3, &USART_ClockInitStructure);

    USART_InitStructure.USART_BaudRate = 2400;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //FIXME tx only should be enough
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;


    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);
}

void writeByte(uint8_t b)
{
	while ((USART3->SR & USART_SR_TXE) == 0);  //blocks until previous byte was sent
	USART3->DR = b;
}

void writeCommand(const PelcoD& pelco)
{
	for(int i = 0; i < 7; ++i)
	{
		writeByte(pelco.data[i]);
	}
}

void playSequence(SequenceEntry* entries, uint16_t size)
{
	for(int i = 0; i < size; ++i)
	{
		writeCommand(*entries[i].cmd);
		Clock::delayMs(entries[i].delayAfterMs);
	}
}


