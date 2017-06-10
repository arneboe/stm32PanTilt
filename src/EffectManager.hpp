#pragma once
#define MAX_NUM_EFFECTS 30
#include "Led.hpp"

/** This method is called repeatedly while an effect is active
 *  @param dt time since last call in ms
 *  @param speed How fast the effect should execute (0=stand still, 255=as fast as possible)
 *  @param leds the led array
 *  @param numLeds number of leds*/
typedef void(*UpdateFunc)(uint8_t dt, uint8_t speed, uint8_t param1, uint8_t param2, Led* leds, uint16_t numLeds);

class EffectManager
{
public:
	EffectManager();

	void addEffect(UpdateFunc updateFunc);

	/** @param effectId Effect (effectId % numEffects) is executed. */
	void execute(uint8_t effectId, uint8_t dt, uint8_t speed, uint8_t param1, uint8_t param2, Led* leds, uint16_t numLeds);

private:
	UpdateFunc effects[MAX_NUM_EFFECTS];
	uint16_t numeffects = 0;
};
