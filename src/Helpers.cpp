#include "Helpers.hpp"

int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max) {
  return ((x - in_min) * (out_max - out_min) + out_min * (in_max - in_min)) / (in_max - in_min);
}

int16_t map2(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max)
{
 return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
}

void setAllHue(Led* leds, int16_t numLeds, uint8_t h)
{
	for(int i = 0; i < numLeds; ++i)
	{
		leds[i].setHue(h);
	}
}


void setAllRGB(Led* leds, int16_t numLeds, uint8_t r, uint8_t g, uint8_t b)
{
	for(int i = 0; i < numLeds; ++i)
	{
		leds[i].r = r;
		leds[i].g = g;
		leds[i].b = b;
	}
}
