/*
 * workMode.cpp
 *
 * Created: 27.11.2020 23:44:06
 *  Author: Seviged
 */ 

#include "workMode.h"
#include "Settings/settings.h"
#include "Watchdog/watchdog.h"

#include "Modes/WorkModeStatic.h"
#include "Modes/WorkModeSimple.h"
#include "Modes/WorkModeNoise.h"

#include "fireGenLib.h"


#define LUXLEN 10
uint8_t _luxFlag = -1;
uint8_t _isFirst = 1;
unsigned long _luxMass[LUXLEN];

uint8_t _nomLux()
{
	uint16_t sumLux = 0;
	for(int i = 0; i < LUXLEN; ++i)
	{
		sumLux += _luxMass[i];
	}
	
	if (_isFirst) return 255;
	return sumLux / LUXLEN;
}

void _addLux(uint8_t newLux)
{
	_luxFlag++;
	if (_luxFlag == LUXLEN)
	{
		_luxFlag = 0;
		_isFirst = 0;
	}
	
	_luxMass[_luxFlag] = newLux;
}

PalleteDefs fireDefines()
{
	PalleteDefs fireDefs;
	fireDefs.hue_start = 15;
	fireDefs.hue_gap = 45;
	fireDefs.minBright = 0.5;
	fireDefs.maxBright = 1;
	fireDefs.minSat = 0.9;
	fireDefs.maxSat = 1;
	return fireDefs;
}

PalleteDefs discoDefines()
{
	PalleteDefs fireDefs;
	fireDefs.hue_start = 0;
	fireDefs.hue_gap = 360;
	fireDefs.minBright = 0.5;
	fireDefs.maxBright = 1;
	fireDefs.minSat = 0.9;
	fireDefs.maxSat = 1;
	return fireDefs;
}

WorkModeNoise fireNoizMode(fireDefines(), 20);
WorkModeNoise discoNoizMode(discoDefines(), 30);
WorkModeSimple simpleMode;
WorkModeStatic staticMode;

WorkMode::WorkMode()
{
	_minBrightness = readSettings(EE_MIN_BRIGHTNESS);
	_maxLuxValue = readSettings(EE_MAX_LUX_RANGE);
	_currentBrightness = 0xFF;
	_toBrightness = 0.2;

	
	defaultMode = &simpleMode;
	mode = defaultMode;
	_handMode = false;
}

void WorkMode::_changeMode(uint8_t nextmode)
{
	_handMode = true;
	if (nextmode == 1)
	{
		mode = &staticMode;
	}
	else if (nextmode == 2)
	{
		mode = &fireNoizMode;	
	}
	else if (nextmode == 3)
	{
		mode = &discoNoizMode;
	}
	else
	{
		mode = defaultMode;
	}
}

void WorkMode::onMainTimerLuxUpdate(unsigned long Lux)
{
	if(Lux < _maxLuxValue && _minBrightness + Lux * 11 <= 255)
	{
		_currentBrightness = _minBrightness + Lux * 11;
	}
	else
	{ 
		_currentBrightness = 0xff;
	}

	_lastLux = Lux;
	_addLux(_currentBrightness);
	WDT_reset();
}

void WorkMode::onMainTimerTimeUpdate(uint16_t intTime)
{
	if(_handMode)
	{
		WDT_reset();
		return;
	}

	if(intTime > 830 && intTime < 1030)
	{
		mode = &fireNoizMode;	
	}
	else if (intTime > 1500 && intTime < 1800)
	{
		mode = &discoNoizMode;
	}
	else if (intTime > 2300 || intTime < 700)
	{
		mode = &staticMode;
	}
	else
	{
		mode = defaultMode;
	}
	//change modes here
	WDT_reset();
}

void WorkMode::onMainTimerChangeColor()
{
	//nope(child mode request)
	mode->onTimerChangeColor();
	WDT_reset();
}

void WorkMode::onMainTimerBlinkDot()
{
	double value = (double)_nomLux() / 0xFF;
	
	const double pitch = 0.01;
	if (_toBrightness > value - pitch)
	{
		_toBrightness -= pitch;
	}
	else if(_toBrightness < value + pitch)
	{
		_toBrightness += pitch;
	}
	else
	{
		_toBrightness = value;
	}
	static const double minBr = 0.22;
	static const double maxBr = 1.0;
	if(_toBrightness < minBr) _toBrightness = minBr;
	if(_toBrightness > maxBr) _toBrightness = maxBr;

	/// call children mode with Lux and _toBrightness
	mode->onTimerBlinkDot(_lastLux, _toBrightness);

	WDT_reset();
}

void WorkMode::_addModes()
{

}
