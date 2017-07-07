#include "AntsEffect.hpp"
#include "Helpers.hpp"
#include "Hue.hpp"
#include "LedBuffer.hpp"

#define MIN_CPS 1
#define MAX_CPS 100

struct State
{
	float shiftSum = 0;
	int16_t currentShift = 0;
};

static State s;

void updateAnts(uint8_t dt, uint8_t speed, uint8_t param1, uint8_t param2, Led* leds, uint16_t numLeds)
{
	//param1 = hue
	//param2 = distance between ants


	const uint8_t hue = param1;
	const uint8_t dist = map(param2, 0, 255, 1, 6);
//	printf_("hue: %d, hueChange: %f, speed: %d, cps: %d, dt: %d\n", s.hue.get(), hueChange, speed, cps, dt);


	//draw pattern in buffer
	bool on = true;
	uint16_t onCount = 0;
	uint16_t offCount = 0;
	for(int i = 0; i < numLeds; ++i)
	{
		if(on)
		{
			Buffer::ledBuffer[i].setHue(hue);
			++onCount;
			if(onCount >= dist)
			{
				on = false;
				onCount = 0;
			}
		}
		else
		{
			Buffer::ledBuffer[i].r = 0;
			Buffer::ledBuffer[i].g = 0;
			Buffer::ledBuffer[i].b = 0;
			++offCount;
			if(offCount >= dist)
			{
				on = true;
				offCount = 0;
			}
		}
	}

	const uint16_t cps = map(speed, 0, 255, MIN_CPS, MAX_CPS);
	const float shiftChange = cps / 1000.0f * dt;
	s.shiftSum += shiftChange;

	if(s.shiftSum > 1)
	{
		const int16_t shiftValue = (int16_t)s.shiftSum;
		s.shiftSum -= shiftValue;
		s.currentShift = (s.currentShift + shiftValue) % numLeds;

	}

	shift(Buffer::ledBuffer, leds, numLeds, s.currentShift);
	//shift(Buffer::ledBuffer, leds, 30, s.currentShift);
	//shift(Buffer::ledBuffer + 30, leds + 30, 30, -s.currentShift);


}
