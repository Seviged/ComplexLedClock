#include "WorkModeSimple.h"

WorkModeSimple::WorkModeSimple()
{
	//color.h = 0;
	//color.s = 1;
	//color.v = 1;
}

void WorkModeSimple::onTimerChangeColor()
{
	color.h++;
	if (color.h > 360)
	{
		color.h = 0;
	}
}

void WorkModeSimple::onTimerBlinkDot(const unsigned long &lux, const double &toBrightness)
{
	color.v = toBrightness;
	static const double minBr = 0.2;
	if(toBrightness < minBr) color.v = minBr;
	updateColorBrightness(color);
}

void WorkModeSimple::loadMode(const ModeDescription &descr)
{
	color = descr.data.simpleDefs.color;
}
