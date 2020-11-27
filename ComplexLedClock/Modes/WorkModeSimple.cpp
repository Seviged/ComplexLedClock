#include "WorkModeSimple.h"

WorkModeSimple::WorkModeSimple()
{
	color.h = 0;
	color.s = 1;
	color.v = 1;
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
	updateColorBrightness(color);
}
