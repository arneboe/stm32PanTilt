#include "ColorPulse.hpp"
#include "../libfixmath/fixmath.h"
#include "../BrightnessLUT.hpp"
#include "../Helpers.hpp"

#define MIN_CPS 1 //changes per Second
#define MAX_CPS 1300
#define MIN_BRIGHTNESS 50


struct State
{
	float brightness = 0.0;
	int8_t dir = 1;
};

static State s;

//Fix16 zero(0.0);
void setPulseBrightness(uint8_t dt, uint8_t pulseBrightness, Led* leds, uint16_t numLeds)
{
	Fix16 bri;
	if(pulseBrightness < 3)
	{
		bri = Fix16(1.0f);
	}
	else
	{
		const float cps = map2(pulseBrightness, 3, 255, MIN_CPS, MAX_CPS);
		const float brightnessChange = cps / 1000.0f * dt;

		s.brightness += brightnessChange * s.dir;
		if(s.brightness >= 255)
		{
			s.dir = -1;
			s.brightness = 255;
		}
		else if(s.brightness <= MIN_BRIGHTNESS)
		{
			s.dir = 1;
			s.brightness = MIN_BRIGHTNESS;
		}
		bri = BrightnessLUT::data[(int)s.brightness];
	}

	for(int i = 0; i < numLeds; ++i)
	{
		leds[i].setBrightness(bri);
	}
}
