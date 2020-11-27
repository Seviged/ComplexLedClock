#include "WorkModeNoise.h"
#include "fireGenLib.h"

WorkModeNoise::WorkModeNoise(const PalleteDefs &defs, uint8_t delay)
{
	_delay = delay;
	pallete = CRGBPalette16(
	getFireColor(0 * 16, defs),
	getFireColor(1 * 16, defs),
	getFireColor(2 * 16, defs),
	getFireColor(3 * 16, defs),
	getFireColor(4 * 16, defs),
	getFireColor(5 * 16, defs),
	getFireColor(6 * 16, defs),
	getFireColor(7 * 16, defs),
	getFireColor(8 * 16, defs),
	getFireColor(9 * 16, defs),
	getFireColor(10 * 16, defs),
	getFireColor(11 * 16, defs),
	getFireColor(12 * 16, defs),
	getFireColor(13 * 16, defs),
	getFireColor(14 * 16, defs),
	getFireColor(15 * 16, defs)
	);
}

void WorkModeNoise::onTimerBlinkDot(const unsigned long &lux, const double &toBrightness)
{
	fillAndUpdateByPalleteWithDelay(pallete, _delay);
}
