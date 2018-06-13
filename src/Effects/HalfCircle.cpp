#include "Rotate1.hpp"
#include "Helpers.hpp"
#include "Hue.hpp"
#include "LedBuffer.hpp"


#define MIN_CPS 1
#define MAX_CPS 100
#define MAX_TAIL_SIZE (numLeds / 2 - 1)

struct State
{
  float shiftSum = 0;
  float shiftSum2 = 0;
  int16_t currentShift = 0;
  int16_t currentShift2 = 0;
};

static State s;


void updateHalfCircle(uint8_t dt, uint8_t speed, uint8_t param1, uint8_t param2, Led* leds, uint16_t numLeds)
{
  //param1 = hue
  //param2 = hue2

  const uint8_t hue1 = param1;
  const uint8_t hue2 = param2;
//  printf_("hue: %d, hueChange: %f, speed: %d, cps: %d, dt: %d\n", s.hue.get(), hueChange, speed, cps, dt);

  //FIXME only redraw if params changed!!!
  //draw pattern in buffer
  for(int i = 0; i < numLeds; ++i)
  {
    Buffer::ledBuffer[i].setBlack();
  }

  const uint16_t ring2 = numLeds / 2;
  const uint16_t ring4 = numLeds / 4;

  //back half ring
  for(int i = 0; i < ring4; ++i)
  {
    Buffer::ledBuffer[i].setHue(hue1);
  }

  //front half ring
  for(int i = ring2 + ring4; i < numLeds; ++i)
  {
    Buffer::ledBuffer[i].setHue(hue2);
  }


  //shift back
  const uint16_t cps = map(speed, 0, 255, MIN_CPS, MAX_CPS);
  const float shiftChange = cps / 1000.0f * dt;
  s.shiftSum += shiftChange;

  if(s.shiftSum > 1)
  {
    const int16_t shiftValue = (int16_t)s.shiftSum;
    s.shiftSum -= shiftValue;
    s.currentShift = (s.currentShift + shiftValue) % numLeds;

  }
  shift(Buffer::ledBuffer, leds, numLeds / 2, -s.currentShift);


  //shift front
  //front shifts with slightly different speed than back
  s.shiftSum2 += shiftChange * 0.9f;

  if(s.shiftSum2 > 1)
  {
    const int16_t shiftValue = (int16_t)s.shiftSum2;
    s.shiftSum2 -= shiftValue;
    s.currentShift2 = (s.currentShift2 + shiftValue) % numLeds;

  }
  shift(Buffer::ledBuffer + ring2, leds + ring2, numLeds / 2, s.currentShift2);

}
