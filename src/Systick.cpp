#include "Clock.hpp"
extern "C" {
void SysTick_Handler()
{
	//update clock by 1 ms
	Clock::tick(1);
}
}
