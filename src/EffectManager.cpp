/*
 * EffectManager.cpp
 *
 *  Created on: Jun 5, 2017
 *      Author: arne
 */

#include "EffectManager.hpp"
#include "printf.h"
#include "Helpers.hpp"

EffectManager::EffectManager() {
	// TODO Auto-generated constructor stub

}


void EffectManager::addEffect(UpdateFunc updateFunc)
{
	if(numeffects < MAX_NUM_EFFECTS)
	{
		effects[numeffects] = updateFunc;
		++numeffects;
	}
	else
	{
		//FIXME assert abort
	}
}

void EffectManager::execute(uint8_t effectId, uint8_t dt, uint8_t speed, uint8_t param1, uint8_t param2, Led* leds, uint16_t numLeds)
{
	if(numeffects == 0)
		return;

	const uint8_t id = map2(effectId, 0, 255, 0, numeffects-1);

	effects[id](dt, speed, param1, param2, leds, numLeds);
}
