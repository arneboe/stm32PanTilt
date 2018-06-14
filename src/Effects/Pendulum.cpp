#include "Rotate1.hpp"
#include "Helpers.hpp"
#include "Hue.hpp"
#include "LedBuffer.hpp"
#include "printf.h"


#define MIN_CPS 1
#define MAX_CPS 100

struct Pendulum {

  float angle = M_PI;
  float aVel = 0.1f;
  float aAcc = 0.0f;
  const float g = 9.81f;
  float len = 1.0f;

  //dt in ms
  void step(uint8_t dt)
  {
    aAcc = (-g / len) * sinf(angle);// + 0.02f;
    aVel += aAcc * dt / 1000.0f;
    angle += aVel * dt / 1000.0f;
  }
};



struct State
{
  Pendulum p;
};

static State s;

//normalize nd convert to deg
float convertAndNormalize(float angleRad){

  const float deg = angleRad * 180 / M_PI;
  float x = fmodf(deg, 360);
  if(x < 0)
      x += 360;
  return x;
}

void updatePendulum(uint8_t dt, uint8_t speed, uint8_t param1, uint8_t param2, Led* leds, uint16_t numLeds)
{

  //param1 = color
  //param2 = fade to

 s.p.step(dt);//update pendulum state

 const float angleDeg = convertAndNormalize(s.p.angle); //convert to degree and normalize to [0..360]

 const float degStep = 360.0f / (numLeds / 2.0f);

 for(int i = 0; i < numLeds; ++i)
 {
   leds[i].setBlack();
 }

 const int ledIndex = degStep / 2.0f + angleDeg / degStep;
 printf_("angleDeg: %f, degStep: %f, i: %d\n", angleDeg, degStep, ledIndex);

 const uint8_t offset = 10;
 const uint8_t hue = param1;
 const uint8_t totalHueShift = param2;

 const uint8_t width = 4;
 const Fix16 hueShiftPerPixel = Fix16(totalHueShift) / Fix16(width);

 Fix16 currentHueShift((uint8_t)0);

  leds[(ledIndex + offset) % numLeds].setHue(hue);
  currentHueShift += hueShiftPerPixel;
  leds[(ledIndex + offset + 1) % numLeds].setHue(hue + currentHueShift.toUint8());
  leds[(ledIndex + offset - 1) % numLeds].setHue(hue + currentHueShift.toUint8());
  currentHueShift += hueShiftPerPixel;
  leds[(ledIndex + offset + 2) % numLeds].setHue(hue + currentHueShift.toUint8());
  leds[(ledIndex + offset - 2) % numLeds].setHue(hue + currentHueShift.toUint8());
  currentHueShift += hueShiftPerPixel;
  leds[(ledIndex + offset + 3) % numLeds].setHue(hue + currentHueShift.toUint8());
  leds[(ledIndex + offset - 3) % numLeds].setHue(hue + currentHueShift.toUint8());


 //for(int i = 0; i < numLeds / 4 - 4; ++i) {
 //  leds[(ledIndex + numLeds/2 + offset + 1) % numLeds].setHue(param1);
 //}

}
