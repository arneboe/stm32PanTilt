#pragma once
#include "Led.hpp"
#define NUM_LEDS 60 //FIXME duplicate CODE!!

/**Contains memory that can be reused by everyone  */
class Buffer
{
public:
	static Led ledBuffer[NUM_LEDS];
};
