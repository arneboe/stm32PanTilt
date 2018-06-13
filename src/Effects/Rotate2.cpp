#include "Rotate1.hpp"
#include "Helpers.hpp"
#include "Hue.hpp"
#include "LedBuffer.hpp"


#define MIN_CPS 1
#define MAX_CPS 200
#define MAX_TAIL_SIZE (numLeds / 2 - 1)

struct State
{
  float shiftSum = 0;
  int16_t currentShift = 0;
};

static State s;


void updateRotate2(uint8_t dt, uint8_t speed, uint8_t param1, uint8_t param2, Led* leds, uint16_t numLeds)
{
  //param1 = hue
  //param2 = size

  const uint8_t hue = param1;
  const uint8_t size = map(param2, 0, 255, 1, MAX_TAIL_SIZE);
//  printf_("hue: %d, hueChange: %f, speed: %d, cps: %d, dt: %d\n", s.hue.get(), hueChange, speed, cps, dt);

  //determine parallel start points
  int16_t l[4] = {0, 0, 0, 0};
  line(leds, numLeds, 0, 0, l);
  //parallel start points are at index 0 and 2

  //FIXME only redraw if params changed!!!
  //draw pattern in buffer
  for(int i = 0; i < numLeds; ++i)
  {
    Buffer::ledBuffer[i].setBlack();
  }

  lineWithTail(Buffer::ledBuffer, numLeds, 0, size, hue);
  lineWithTail(Buffer::ledBuffer, numLeds, numLeds/2, size, hue);


  //shift buffer
  const uint16_t cps = map(speed, 0, 255, MIN_CPS, MAX_CPS);
  const float shiftChange = cps / 1000.0f * dt;
  s.shiftSum += shiftChange;

  if(s.shiftSum > 1)
  {
    const int16_t shiftValue = (int16_t)s.shiftSum;
    s.shiftSum -= shiftValue;
    s.currentShift = (s.currentShift + shiftValue) % numLeds;

  }

  shift(Buffer::ledBuffer, leds, numLeds, -s.currentShift);
}
