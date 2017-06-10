#pragma once
#include "Led.hpp"


int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


uint16_t map2(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  return ((x - in_min) * (out_max - out_min) + out_min * (in_max - in_min)) / (in_max - in_min);
}


void setAllHue(Led* leds, uint16_t numLeds, uint8_t h)
{
	for(int i = 0; i < numLeds; ++i)
	{
		leds[i].setHue(h);
	}
}
