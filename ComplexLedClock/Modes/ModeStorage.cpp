#include "ModeStorage.h"
#include "LedClock.h"

 ModeStorage::ModeStorage()
 {
	ModeDescription defaultModeDescription;
	defaultModeDescription.type = MODE_STATIC;
	hsv color;
	color.h = 0;
	color.s = 0;
	color.v = 0.01;
	defaultModeDescription.data.staticDefs.color = color;

	addMode(defaultModeDescription);
 }

void ModeStorage::addMode(ModeDescription &descr)
{
	if (_tail < MAX_MODE_COUNT)
	{
		_storage[_tail] = descr;
		++_tail;
	}
}

ModeDescription & ModeStorage::getMode(uint8_t modeNumber)
{
	_lastMode = modeNumber;
	if(modeNumber >= 0 && modeNumber < _tail)
	{	
		return _storage[modeNumber];
	}

	return _storage[0];
}
