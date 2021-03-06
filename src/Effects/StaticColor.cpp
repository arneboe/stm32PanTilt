#include "StaticColor.hpp"
#include "Helpers.hpp"

void updateStaticColor(uint8_t dt, uint8_t speed, uint8_t param1, uint8_t param2, Led* leds, uint16_t numLeds)
{
	if(param1 >= 253)
	{
		setAllWhite(leds, numLeds);
	}
	else
	{
		setAllHue(leds, numLeds, param1);
	}
}
