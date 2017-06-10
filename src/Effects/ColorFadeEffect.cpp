#include "ColorFadeEffect.hpp"
#include "Helpers.hpp"
#include "printf.h"
#include "Hue.hpp"

#define MIN_CPS 1
#define MAX_CPS 500

struct State
{
	Hue hue;
};

static State s;

void updateColorFade(uint8_t dt, uint8_t speed, Led* leds, uint16_t numLeds)
{
/**
 * Fastest speed = 500 changes per second
 * Slowest speed = 1 change per second
 */
	const uint16_t cps = map2(speed, 0, 255, MIN_CPS, MAX_CPS);
	const float hueChange = cps / 1000.0f * dt;
	s.hue.update(hueChange);
	printf_("hue: %d, hueChange: %f, speed: %d, cps: %d, dt: %d\n", s.hue.get(), hueChange, speed, cps, dt);


	setAllHue(leds, numLeds, s.hue.get());
}
