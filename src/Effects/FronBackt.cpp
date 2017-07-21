#include "FrontBack.hpp"
#include "Helpers.hpp"
#include "Hue.hpp"
#include "LedBuffer.hpp"

#define MAX_TIME 1000
#define MIN_TIME 4

struct State
{
	bool front = true;
	float lastChangeTime = 0;
	uint16_t timeAcc = 0;
};

static State s;

void updateFrontBack(uint8_t dt, uint8_t speed, uint8_t param1, uint8_t param2, Led* leds, uint16_t numLeds)
{
	//param1 = left hue
	//param2 = right hue


	const uint8_t hue = param1;
	const uint8_t hue2 = param2;
//	printf_("hue: %d, hueChange: %f, speed: %d, cps: %d, dt: %d\n", s.hue.get(), hueChange, speed, cps, dt);


	const uint16_t leftStart = 0;
	const uint16_t leftEnd = numLeds / 2;;
	const uint16_t rightStart = leftEnd;
	const uint16_t rightEnd = numLeds;
	const uint16_t time = map2(speed, 0, 255, MAX_TIME, MIN_TIME);

	s.timeAcc += dt;

	if(s.timeAcc >= time)
	{
		s.timeAcc = 0;
		if(s.front)
		{
			s.front = false;
			for(int i = leftStart; i < leftEnd; ++i)
			{
				leds[i].setBlack();
			}
			for(int i = rightStart; i < rightEnd; ++i)
			{
				leds[i].setHue(hue);
			}
		}
		else
		{
			s.front = true;
			for(int i = leftStart; i < leftEnd; ++i)
			{
				leds[i].setHue(hue2);
			}
			for(int i = rightStart; i < rightEnd; ++i)
			{
				leds[i].setBlack();
			}
		}
	}
}
