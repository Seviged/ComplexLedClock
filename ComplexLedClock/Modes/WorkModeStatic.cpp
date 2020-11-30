#include "WorkModeStatic.h"

WorkModeStatic::WorkModeStatic()
{
	
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

void WorkModeStatic::loadMode(const ModeDescription &descr)
{
	color = descr.data.staticDefs.color;
	//color.h = 0;
	//color.s = 0;
	//color.v = 0.01;
}
