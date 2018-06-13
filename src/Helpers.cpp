#include "Helpers.hpp"
#include "BrightnessLUT.hpp"

int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max) {
  return ((x - in_min) * (out_max - out_min) + out_min * (in_max - in_min)) / (in_max - in_min);
}

int16_t map2(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max)
{
 return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
}

void setAllHue(Led* leds, int16_t numLeds, uint8_t h)
{
	for(int i = 0; i < numLeds; ++i)
	{
		leds[i].setHue(h);
	}
}


void setAllWhite(Led* leds, int16_t numLeds)
{
  for(int i = 0; i < numLeds; ++i)
  {
    leds[i].setWhite();
  }
}


void setAllRGB(Led* leds, int16_t numLeds, uint8_t r, uint8_t g, uint8_t b)
{
	for(int i = 0; i < numLeds; ++i)
	{
		leds[i].r = r;
		leds[i].g = g;
		leds[i].b = b;
	}
}


void shift(const Led* in, Led* out, int16_t numLeds, int8_t shift)
{
	for(int i = 0; i < numLeds; ++i)
	{
		//+ 2 * numLeds to reach positive range for negative shifts
		const int16_t targetIndex = (i + 2 * numLeds + shift) % numLeds;
		out[targetIndex].r = in[i].r;
		out[targetIndex].g = in[i].g;
		out[targetIndex].b = in[i].b;
	}

}

int16_t getLineMaxShift(int16_t numLeds)
{
	return numLeds / 8;
}


/** Draws a line a "dot" with a tail, i.e. a fading line */
void lineWithTail(Led* leds, int16_t numLeds, uint16_t startIdx, uint16_t tailSize, uint8_t hue)
{
  const int16_t brightnessReductionStep = 256 / tailSize;

  //const uint8_t hueShift = 5;
  Fix16 brightness((uint8_t)1);
  for(int j = startIdx; j < startIdx + tailSize; ++j)
  {
    leds[j % numLeds].setHue(hue); //Fixme add some hue shift?
    leds[j % numLeds].setBrightness(BrightnessLUT::data[255 - (j - startIdx) * brightnessReductionStep]);
  }

}


/** Intersects a horizontal line with the circle. Returns indices of both intersections.
 * @param rot The angle of the start location.
 * @param shift Height difference from the start location to the line */
void line(Led* leds, int16_t numleds, uint32_t rot, int16_t shift, int16_t* result)
{
	const int16_t halfRing = numleds / 4;
	//const int16_t maxShift = getLineMaxShift(numleds);

	result[0] = (0 * halfRing + rot) % numleds;
	result[1] = (1 * halfRing + rot) % numleds;
	result[2] = (2 * halfRing + rot) % numleds;
	result[3] = (3 * halfRing + rot) % numleds;

	if(shift > 0)
	{
		result[0] = (result[0] + shift + numleds) % numleds;
		result[1] = (result[1] - shift + numleds) % numleds;
		result[2] = (result[2] + shift + numleds) % numleds;
		result[3] = (result[3] - shift + numleds) % numleds;
	}
	else if(shift < 0)
	{
		shift *= -1;
		result[0] = (result[0] - shift + numleds) % numleds;
		result[1] = (result[1] + shift + numleds) % numleds;
		result[2] = (result[2] - shift + numleds) % numleds;
		result[3] = (result[3] + shift + numleds) % numleds;
	}


}




