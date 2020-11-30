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
	WorkModeNoise();
	void onTimerChangeColor() override {}
	void onTimerBlinkDot(const unsigned long &lux, const double &toBrightness) override;
	void loadMode(const ModeDescription &descr) override;
private:
	CRGBPalette16 pallete;
	uint8_t _delay;
	bool _correctBrightness = false;
};



#endif /* WORKMODENOISE_H_ */