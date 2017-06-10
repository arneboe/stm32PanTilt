#pragma once
#include <cmath>


class Hue
{
private:
	float value = 0;
public:
	void update(float change)
	{
		value += change;
		if(value < 0) value = 255;
		if(value > 255) value = 0;
	}

	uint8_t get()
	{
		return (uint8_t)round(value);
	}

};
