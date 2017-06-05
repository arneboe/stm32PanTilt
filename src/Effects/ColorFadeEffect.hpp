#pragma once
#include <cstdint>
class Led;

void updateColorFade(uint8_t dt, uint8_t speed, Led* leds, uint16_t numLeds);
