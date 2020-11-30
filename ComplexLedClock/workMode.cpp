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

NoizeModeDefs fireDefines()
{
	NoizeModeDefs fireDefs;
	fireDefs.hue_start = 15;
	fireDefs.hue_gap = 45;
	fireDefs.minBright = 0.5;
	fireDefs.maxBright = 1;
	fireDefs.minSat = 0.9;
	fireDefs.maxSat = 1;
	fireDefs.correctBrightness = false;
	fireDefs.delay = 20;
	return fireDefs;
}

NoizeModeDefs discoDefines()
{
	NoizeModeDefs fireDefs;
	fireDefs.hue_start = 0;
	fireDefs.hue_gap = 360;
	fireDefs.minBright = 0.5;
	fireDefs.maxBright = 1;
	fireDefs.minSat = 0.9;
	fireDefs.maxSat = 1;
	fireDefs.correctBrightness = true;
	fireDefs.delay = 30;
	return fireDefs;
}


WorkModeSimple simpleMode;
WorkModeStatic staticMode;
WorkModeNoise  noizeMode;


WorkMode::WorkMode()
{
	_minBrightness = readSettings(EE_MIN_BRIGHTNESS);
	_maxLuxValue = readSettings(EE_MAX_LUX_RANGE);
	_currentBrightness = 0xFF;
	_toBrightness = 0.2;

	_addModes();

	setMode(0);
}

void WorkMode::_changeMode(ModeType nextmode)
{
	if (nextmode == MODE_NOIZE)
	{
		mode = &noizeMode;
	}
	else if (nextmode == MODE_SIMPLE)
	{
		mode = &simpleMode;	
	}
	else //(nextmode == MODE_STATIC)
	{
		mode = &staticMode;
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
	if(intTime > 830 && intTime < 1030)
	{
		setMode(2);
	}
	else if (intTime > 1500 && intTime < 1515)
	{
		setMode(3);
	}
	else if (intTime > 2300 || intTime < 700)
	{
		setMode(0);
	}
	else
	{
		setMode(1);
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

	static const double maxBr = 1.0;
	if(_toBrightness > maxBr) _toBrightness = maxBr;

	/// call children mode with Lux and _toBrightness
	mode->onTimerBlinkDot(_lastLux, _toBrightness);

	WDT_reset();
}

void WorkMode::setMode(uint8_t mod)
{
	if (mod == _storage.getLastModeNumber())
	{
		return;
	}

	auto currentMode = _storage.getMode(mod);
	_changeMode(currentMode.type);
	mode->loadMode(currentMode);
}

void WorkMode::_addModes()
{
	ModeDescription fire;
	fire.type = MODE_NOIZE;
	fire.data.noizeDefs = fireDefines();

	ModeDescription disco;
	disco.type = MODE_NOIZE;
	disco.data.noizeDefs = discoDefines();

	ModeDescription simple;
	simple.type = MODE_SIMPLE;
	hsv clr;
	clr.h = 0;
	clr.s = 1;
    clr.v = 0.2;
	simple.data.simpleDefs.color = clr;

	_storage.addMode(simple);	//1
	_storage.addMode(fire);		//2
	_storage.addMode(disco);	//3
}
