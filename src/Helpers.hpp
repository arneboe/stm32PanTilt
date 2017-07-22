#pragma once
#include "Led.hpp"



int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max);


int16_t map2(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max);

void setAllHue(Led* leds, int16_t numLeds, uint8_t h);

void setAllRGB(Led* leds, int16_t numLeds, uint8_t r, uint8_t g, uint8_t b);


void shift(const Led* in, Led* out, int16_t numLeds, int8_t shift);


//result should have at least size 4
void line(Led* leds, int16_t numleds, uint32_t rot, int16_t shift, int16_t* result);

int16_t getLineMaxShift(int16_t numLeds);
