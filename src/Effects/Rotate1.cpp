#include "Rotate1.hpp"
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


void updateRotate(uint8_t dt, uint8_t speed, uint8_t param1, uint8_t param2, Led* leds, uint16_t numLeds)
{
	//param1 = hue
	//param2 = size

	const uint8_t hue = param1;
	const uint8_t size = map(param2, 0, 255, 1, 10);
//	printf_("hue: %d, hueChange: %f, speed: %d, cps: %d, dt: %d\n", s.hue.get(), hueChange, speed, cps, dt);


	const int firstLimit = numLeds / 2.0;


	//draw pattern in buffer
	bool on = true;
	uint16_t onCount = 0;
	uint16_t offCount = 0;
	for(int i = 0; i < numLeds; ++i)
	{
		if(i < size ||
		   (i >= firstLimit && i < firstLimit + size))
		{
			Buffer::ledBuffer[i].setHue(hue);
		}
		else
		{
			Buffer::ledBuffer[i].setBlack();
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

	shift(Buffer::ledBuffer, leds, firstLimit, s.currentShift);
	shift(Buffer::ledBuffer, leds + firstLimit, firstLimit, -s.currentShift);
}
