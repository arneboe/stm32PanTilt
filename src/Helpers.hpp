#pragma once
#include "Led.hpp"



uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

void setAllHue(Led* leds, uint16_t numLeds, uint8_t h);
