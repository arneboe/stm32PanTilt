#include "ColorPulse.hpp"
#include "../libfixmath/fixmath.h"
#include "../Helpers.hpp"
#include "../printf.h"

#define MIN_CPM 1 //changes per Minute
#define MAX_CPM 400

struct State
{
	float brightness = 0.0;
	int8_t dir = 1;
	uint8_t hue = 0;
};

static State s;

//Fix16 zero(0.0);

void updateColorPulse(uint8_t dt, uint8_t speed, uint8_t param1,
		              uint8_t param2, Led* leds, uint16_t numLeds)
{
	const float cpm = map2(speed, 0, 255, MIN_CPM, MAX_CPM);
	const float brightnessChange = cpm / 60.0f * dt / 1000.0f;

	s.brightness += brightnessChange * s.dir;
	if(s.brightness >= 1)
	{
		s.dir = -1;
		s.brightness = 1;
	}
	else if(s.brightness <= 0)
	{
		s.dir = 1;
		s.brightness = 0;
	}

	printf_("change: %f ,dir: %d,  speed: %d ,bri: %f\n",brightnessChange, s.dir, speed, s.brightness);

	//FIXME improve performance, not critical right now
	setAllHue(leds, numLeds, s.hue);
	Fix16 bri(s.brightness);
	for(int i = 0; i < numLeds; ++i)
	{
		leds[i].setBrightness(bri);
	}

	//param1 = color
	//param2 = unused
}
