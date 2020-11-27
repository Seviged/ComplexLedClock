/*
 * WorkModeStatic.h
 *
 * Created: 28.11.2020 0:00:41
 *  Author: Seviged
 */ 


#ifndef WORKMODESTATIC_H_
#define WORKMODESTATIC_H_

#include "workMode.h"

class WorkModeStatic : public WorkModeInterface
{
public:
	WorkModeStatic();
	void onTimerChangeColor() {}
	void onTimerBlinkDot(const unsigned long &lux, const double &toBrightness);
private:
	hsv color;
};



#endif /* WORKMODESTATIC_H_ */