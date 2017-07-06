#pragma once
#include <cstdint>
#include "libfixmath/fixmath.h"

/** A lookup table for exponential linear brightness adjustment (which will seem linear to the eye) */
struct BrightnessLUT
{
	static Fix16 data[256];
};

