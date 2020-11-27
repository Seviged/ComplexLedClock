#include "WorkModeStatic.h"

WorkModeStatic::WorkModeStatic()
{
	color.h = 0;
	color.s = 0;
	color.v = 0.01;
}

void WorkModeStatic::onTimerBlinkDot(const unsigned long &lux, const double &toBrightness)
{
	if(lux < 20)
	{
		color.v = 0.01;
	}
	else
	{
		color.v = toBrightness;
	}

	fillAndUpdateBySingleColor(color);
}
