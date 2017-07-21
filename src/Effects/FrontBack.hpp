#pragma once
#include <cstdint>
class Led;

void updateFrontBack(uint8_t dt, uint8_t speed, uint8_t param1, uint8_t param2, Led* leds, uint16_t numLeds);
