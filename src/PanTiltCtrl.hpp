#pragma once
#include "pelcoD/PelcoD.hpp"

struct SequenceEntry
{
	PelcoD* cmd;
	uint16_t delayAfterMs;
};

void initPanTilt();
void writeCommand(const PelcoD& pelco);
void playSequence(SequenceEntry* entries, uint16_t size);
