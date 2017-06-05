#include "ColorFadeEffect.hpp"
#include "Helpers.hpp"

#define MIN_CPS 1
#define MAX_CPS 500

struct State
{
	uint8_t hue = 0;
	uint16_t dtAccu;
};

static State s;

void updateColorFade(uint8_t dt, uint8_t speed, Led* leds, uint16_t numLeds)
{
/**
 * Fastest speed = 500 changes per second
 * Slowest speed = 1 change per second
 */
	const uint16_t cps = map(speed, 0, 255, MIN_CPS, MAX_CPS);
	const uint16_t stepDt = 1000 / cps;
	s.dtAccu += dt;
	if(s.dtAccu < stepDt)
		return;

	s.dtAccu = 0;
	++s.hue;
	setAllHue(leds, numLeds, s.hue);
}
