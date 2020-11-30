#include "WorkModeNoise.h"
#include "fireGenLib.h"
#include "utils.h"

WorkModeNoise::WorkModeNoise()
{

}

void WorkModeNoise::onTimerBlinkDot(const unsigned long &lux, const double &toBrightness)
{
	uint8_t br = 255;
	if (_correctBrightness)
	{
		br = mapf(toBrightness, 0.0, 1.0, 0.0, 255.0);
		if (br < 0.22)
		{	
			br = 0.22;
		}
	}
	fillAndUpdateByPalleteWithDelay(pallete, _delay, br);
}

void WorkModeNoise::loadMode(const ModeDescription &descr)
{
		_delay = descr.data.noizeDefs.delay;
		_correctBrightness = descr.data.noizeDefs.correctBrightness;
		pallete = CRGBPalette16(
		getFireColor(0 * 16, descr.data.noizeDefs),
		getFireColor(1 * 16, descr.data.noizeDefs),
		getFireColor(2 * 16, descr.data.noizeDefs),
		getFireColor(3 * 16, descr.data.noizeDefs),
		getFireColor(4 * 16, descr.data.noizeDefs),
		getFireColor(5 * 16, descr.data.noizeDefs),
		getFireColor(6 * 16, descr.data.noizeDefs),
		getFireColor(7 * 16, descr.data.noizeDefs),
		getFireColor(8 * 16, descr.data.noizeDefs),
		getFireColor(9 * 16, descr.data.noizeDefs),
		getFireColor(10 * 16, descr.data.noizeDefs),
		getFireColor(11 * 16, descr.data.noizeDefs),
		getFireColor(12 * 16, descr.data.noizeDefs),
		getFireColor(13 * 16, descr.data.noizeDefs),
		getFireColor(14 * 16, descr.data.noizeDefs),
		getFireColor(15 * 16, descr.data.noizeDefs)
		);
}
