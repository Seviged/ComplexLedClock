/*
 * WorkModeNoise.h
 *
 * Created: 28.11.2020 0:00:19
 *  Author: Seviged
 */ 


#ifndef WORKMODENOISE_H_
#define WORKMODENOISE_H_

#include "workMode.h"

class WorkModeNoise : public WorkModeInterface
{
public:
	WorkModeNoise(const PalleteDefs &defs, uint8_t delay);
	void onTimerChangeColor() {}
	void onTimerBlinkDot(const unsigned long &lux, const double &toBrightness);
private:
	CRGBPalette16 pallete;
	uint8_t _delay;
};



#endif /* WORKMODENOISE_H_ */