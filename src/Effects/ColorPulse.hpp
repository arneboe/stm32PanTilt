#pragma once
#include <cstdint>
class Led;

//Modifies the brightness of all leds according to @p pulseBrightness
void setPulseBrightness(uint8_t dt, uint8_t pulseBrightness, Led* leds, uint16_t numLeds);
