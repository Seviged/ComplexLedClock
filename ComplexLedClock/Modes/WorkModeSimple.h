/*
 * WorkModeSimple.h
 *
 * Created: 27.11.2020 23:58:11
 *  Author: Seviged
 */ 


#ifndef WORKMODESIMPLE_H_
#define WORKMODESIMPLE_H_

#include "workMode.h"

class WorkModeSimple : public WorkModeInterface
{
public:
	WorkModeSimple();
	void onTimerChangeColor() override;
	void onTimerBlinkDot(const unsigned long &lux, const double &toBrightness) override;
	void loadMode(const ModeDescription &descr) override;
private:
	hsv color;
};



#endif /* WORKMODESIMPLE_H_ */