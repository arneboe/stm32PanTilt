#pragma once
#include "Led.hpp"



int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max);


int16_t map2(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max);

void setAllHue(Led* leds, int16_t numLeds, uint8_t h);
