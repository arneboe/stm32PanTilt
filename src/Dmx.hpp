#pragma once
#include "stm32f10x.h"

#define DMX_CHANNEL_WIDTH         513
#define RECEIVER_NUMBER            1
#define RESET_VALUE                0
#define BREAK_TO_BREAK_TIME       149
#define MAB_TIME                   1
#define BREAK_TIME                 11
#define TIMER_OVERFLOW_Value     0xFFFF

/* Exported variables --------------------------------------------------------*/
extern bool b_BreakFlag, b_MABFlag, b_PacketFlag, b_DataCorruptFlag, \
  b_InitBreakFlag, b_InitCheckFlag;
extern u8 u8_GloabalOverflowCount, u8_MABOverflowCount;
extern u8 u8_BreakOverflowCount;
extern vu8 vu8_FreezCount, vu8_DMX_Buff[DMX_CHANNEL_WIDTH];
extern u16 u16_DMXChannelCount;
extern u16 u16_BreakFallingCounter, u16_NextBreakFallingCounter, \
  u16_MABRisingCounter, u16_MABFallingCounter;
extern u32 u32_TimingCounter;

void dmxInit();
