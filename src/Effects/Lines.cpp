#include "AntsEffect.hpp"
#include "Helpers.hpp"
#include "Hue.hpp"
#include "LedBuffer.hpp"
#include "BrightnessLUT.hpp"

#define MIN_CPS 1
#define MAX_CPS 100

struct State
{
	int16_t shift = 0;
	uint16_t timeAcc = 0;
};

#define MAX_TIME 300
#define MIN_TIME 4

static State s;

void updateLines(uint8_t dt, uint8_t speed, uint8_t param1, uint8_t param2, Led* leds, uint16_t numLeds)
{
	//param1 = angle
	//param2 = hue

	const uint8_t angle = map2(param1, 0, 255, 0, numLeds/2);
	const uint8_t hue = param2;

	const uint16_t time = map2(speed, 0, 255, MAX_TIME, MIN_TIME);

	s.timeAcc += dt;

	if(s.timeAcc >= time)
	{
		s.timeAcc = 0;
		++s.shift;
		if(s.shift > getLineMaxShift(numLeds))
		{
			s.shift = -getLineMaxShift(numLeds);
		}
		int16_t l[4] = {0, 0, 0, 0};
		line(leds, numLeds, angle, s.shift, l);

		for(int i = 0; i < NUM_LEDS; ++i)
		{
			leds[i].setBlack();
		}

		const int16_t numBrightnessReductions = numLeds / 8;
		const int16_t brightnessReductionStep = 256 / numBrightnessReductions;

		const uint8_t hueShift = 5;
		Fix16 brightness((uint8_t)1);
		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < numBrightnessReductions; ++j)
			{
				leds[l[i] + j].setHue(hue + hueShift * j);
				leds[l[i] + j].setBrightness(BrightnessLUT::data[255 - j * brightnessReductionStep]);
			}
		}
	}
}
