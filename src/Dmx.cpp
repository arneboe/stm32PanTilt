#include "Dmx.hpp"

#define FRAMMING_ERROR_FLAG      0x00000002
#define RXNE_FLAG                0x00000020

ErrorStatus HSEStartUpStatus;
bool b_BreakFlag = false;
bool b_MABFlag = false;
bool b_PacketFlag = false;
bool b_DataCorruptFlag = false;
bool b_InitBreakFlag = false;
bool b_InitCheckFlag = false;
u8 u8_GloabalOverflowCount = RESET_VALUE;
u8 u8_MABOverflowCount = RESET_VALUE;
u8 u8_BreakOverflowCount = RESET_VALUE;
vu8 vu8_FreezCount = RESET_VALUE;
u16 u16_DMXChannelCount = RESET_VALUE;
u16 u16_BreakFallingCounter = RESET_VALUE;
u16 u16_NextBreakFallingCounter = RESET_VALUE;
u16 u16_MABRisingCounter = RESET_VALUE;
u16 u16_MABFallingCounter = RESET_VALUE;
u32 u32_TimingCounter = RESET_VALUE;
vu8 vu8_DMX_Buff[DMX_CHANNEL_WIDTH]={1,0xFF,1,1};
u8 u8_ChannelData;

void RCC_Configuration(void)
{

  /* TIM2 and TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /*GPIO clock enable*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* USART1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* GPIOA Configuration: TIM2 Channel1 and Channel2 in input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


}


void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

  /* 1 bit for pre-emption priority, 3 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Enable the TIM2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Protection from False interrupt */
  b_InitCheckFlag = true;
}



TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_ICInitTypeDef TIM_ICInitStructure;


extern  u8 u8_ChannelData;


void USART1_Intialise(void)
{
  USART_ClockInitTypeDef USART_ClockInitStructure;
  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
  USART_ClockInit(USART1, &USART_ClockInitStructure);

  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 250000;                     /* 250KBaud*/
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_2 ;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx;
  USART_Init(USART1, &USART_InitStructure);

  /* Enable USART1 Receive and Transmit interrupt */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);
}

void Timer2_Intialise(void)
{
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 576;
  TIM_TimeBaseStructure.TIM_ClockDivision =  TIM_CKD_DIV1 ;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* Input Capture Mode configuration: Channel1 */
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0;

  TIM_ICInit(TIM2, &TIM_ICInitStructure);

  /* Input Capture Mode configuration: Channel2 */
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0;

  TIM_ICInit(TIM2, &TIM_ICInitStructure);

  /* Enable IM2 update interrupt*/
  TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);

  /* TIM enable counter */
  TIM_Cmd(TIM2, ENABLE);
}

void dmxInit()
{
  RCC_Configuration();
  GPIO_Configuration();
  USART1_Intialise();
  Timer2_Intialise();
  NVIC_Configuration();

}

u32 u32_StatusRead = 0;
extern "C" {
void USART1_IRQHandler(void)
{
  u8 u8_ReceivedData;

  /* Read the status */
  u32_StatusRead = USART1->SR;

  /* Protection from False interrupt */
  if (b_InitCheckFlag == true)
  {
    if((u32_StatusRead & FRAMMING_ERROR_FLAG) != RESET)
    {
      /* Dummy read to clear flag */
      USART_ReceiveData(USART1);

      if((b_BreakFlag == false) && (b_MABFlag == false))
      {
        u16_BreakFallingCounter = TIM_GetCapture1(TIM2);

        /* Enable TIM2_CH2 Input capture Rissing edge */
        TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
      }
      else if(b_MABFlag == true )
      {
        u16_NextBreakFallingCounter = TIM_GetCapture1(TIM2);

        /* Enable TIM2_CH2 Input capture Rissing edge */
        TIM_ITConfig(TIM2, TIM_IT_CC2,ENABLE);

        u8_BreakOverflowCount = u8_GloabalOverflowCount;
      }
    }
    /* If no framming error but real data received */
    else if(((u32_StatusRead & RXNE_FLAG) != RESET) && (b_MABFlag == true) &&\
      (b_DataCorruptFlag == false))
    {
      if(u16_DMXChannelCount == RESET_VALUE)
      {
        u8_ReceivedData = USART_ReceiveData(USART1);
        if(u8_ReceivedData != RESET_VALUE)
        {
          b_MABFlag = false;
          u8_GloabalOverflowCount = RESET_VALUE;
          b_InitBreakFlag = false;
        }
        else
        {
          vu8_DMX_Buff[u16_DMXChannelCount] = u8_ReceivedData;
          u16_DMXChannelCount++;
        }
      }
      else
      {
        if(u16_DMXChannelCount < DMX_CHANNEL_WIDTH)
        {
          vu8_DMX_Buff[u16_DMXChannelCount] = USART_ReceiveData(USART1);
          u16_DMXChannelCount++;
        }
        else
        {
          u16_DMXChannelCount = RESET_VALUE;
          vu8_DMX_Buff[u16_DMXChannelCount] = USART_ReceiveData(USART1);
        }
      }
    }
  }

  USART_ClearFlag(USART1, USART_IT_FE);
  USART_ReceiveData(USART1);
  USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}


void TIM2_IRQHandler(void)
{
  u8 u8_Count = 1;
  /* Protection from false interrupt */
  if(b_InitCheckFlag == true)
  {
    /* Update Event */
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
      /* Overflow Conter to increase */
      u8_GloabalOverflowCount++;
      TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }

    /* TIM2_CH1 Input capture Falling edge*/
    else if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
    {
      /* Get Capture data at Start bit of Start Code */
      u16_MABFallingCounter = TIM_GetCapture1(TIM2);

      /* Falling edge Input Capture Interrupt Disable */
      TIM_ITConfig(TIM2, TIM_IT_CC1,DISABLE);

      /* There is no overflow of counter in MAB */
      if(u8_GloabalOverflowCount == u8_MABOverflowCount)
      {
        u32_TimingCounter = (u16_MABFallingCounter - u16_MABRisingCounter);
      }
      else    /* At least an overflow of counter in MAB */
      {
        u32_TimingCounter = ((TIMER_OVERFLOW_Value - u16_MABRisingCounter) + \
          u16_MABFallingCounter +1);

        for(u8_Count = 1; u8_Count < (u8_GloabalOverflowCount - u8_MABOverflowCount);\
          u8_Count++)
        {
          u32_TimingCounter = u32_TimingCounter + TIMER_OVERFLOW_Value;
        }
      }

      /* Ckeck for Break Flag */
      if(b_BreakFlag == true)
      {
        /* Checking for valid MAB */
        if((u32_TimingCounter) >= MAB_TIME)
        {
          b_MABFlag = true;
          b_BreakFlag = false;
        }
        else
        {
          b_BreakFlag = false;
          u8_GloabalOverflowCount = RESET_VALUE;
          b_InitBreakFlag = false;
        }
      }
      TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    }

    /* TIM2_CH2 Input capture Rissing edge*/
    else if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
    {
      /* Get Capture data at end of Break (Rising Edge) */
      u16_MABRisingCounter = TIM_GetCapture2(TIM2);

      /* Diable TIM2_CH2 Input capture Rissing edge*/
      TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE);

      /* Very first End of break (Not neccessary a valid break) */
      if(b_InitBreakFlag == false)
      {
        if(u8_GloabalOverflowCount == RESET_VALUE)
        {
          u32_TimingCounter = (u16_MABRisingCounter - u16_BreakFallingCounter);
        }
        else
        {
          u32_TimingCounter = ((TIMER_OVERFLOW_Value - u16_BreakFallingCounter)\
            + u16_MABRisingCounter + 1);
          for(u8_Count = 1; u8_Count < u8_GloabalOverflowCount; u8_Count++)
          {
            u32_TimingCounter = u32_TimingCounter + TIMER_OVERFLOW_Value;
          }
        }

        /* Checking for break time for valid break */
        if((u32_TimingCounter) >= BREAK_TIME)
        {
          TIM_ITConfig(TIM2, TIM_IT_CC1,ENABLE);
          b_BreakFlag = true;
          b_InitBreakFlag = true;
          u8_MABOverflowCount = u8_GloabalOverflowCount;
        }
        else
        {
          u8_GloabalOverflowCount = RESET_VALUE;
        }
      }
      else  /* End of break (Not first break & Not neccessary a valid break) */
      {
        if(u8_GloabalOverflowCount == u8_BreakOverflowCount)
        {
          u32_TimingCounter = (u16_MABRisingCounter - u16_NextBreakFallingCounter);
        }
        else
        {
          u32_TimingCounter = ((TIMER_OVERFLOW_Value - u16_NextBreakFallingCounter)\
            + u16_MABRisingCounter +1);
          for(u8_Count = 1; u8_Count < (u8_GloabalOverflowCount - u8_BreakOverflowCount);\
            u8_Count++)
          {
            u32_TimingCounter = u32_TimingCounter + TIMER_OVERFLOW_Value;
          }
        }

        /* Checking for break time for valid break */
        if((u32_TimingCounter) >= BREAK_TIME)
        {
          /* Enable Falling edge interrupt for Start bit of start code */
          TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

          u8_GloabalOverflowCount = (u8_GloabalOverflowCount - u8_BreakOverflowCount);

          b_MABFlag = false;
          u16_DMXChannelCount = RESET_VALUE;

          if((u8_BreakOverflowCount) == RESET_VALUE)
          {
            u32_TimingCounter = (u16_NextBreakFallingCounter-u16_BreakFallingCounter);
          }
          else
          {
            u32_TimingCounter = ((TIMER_OVERFLOW_Value - u16_BreakFallingCounter)\
              + u16_NextBreakFallingCounter +1);
            for(u8_Count =1;  u8_Count  < u8_BreakOverflowCount; u8_Count++)
            {
              u32_TimingCounter = u32_TimingCounter + TIMER_OVERFLOW_Value;
            }
          }
          /* Checking for valid break time for valid break */
          if((u32_TimingCounter) >= BREAK_TO_BREAK_TIME)
          {
            b_PacketFlag = true;
            vu8_FreezCount = u16_DMXChannelCount - 1;
          }
          else
          {
            b_PacketFlag = false;
          }

          b_BreakFlag = true;
          b_InitBreakFlag = true;
          u16_BreakFallingCounter = u16_NextBreakFallingCounter;
        }
        else
        {
          b_DataCorruptFlag = true;
          vu8_FreezCount = u16_DMXChannelCount - 1;
        }
      }

      TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
    }
  }
  TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
  TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

}
