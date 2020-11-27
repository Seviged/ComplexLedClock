/*
 * workMode.h
 *
 * Created: 27.11.2020 23:44:15
 *  Author: Seviged
 */ 


#ifndef WORKMODE_H_
#define WORKMODE_H_

#include <avr/io.h>
#include "LedClock.h"

class WorkModeInterface
{
public:
	virtual void onTimerChangeColor() = 0;
	virtual void onTimerBlinkDot(const unsigned long &lux, const double &toBrightness) = 0;
};

class WorkMode
{
public:
	WorkMode();
	void _changeMode(uint8_t mode);
	void onMainTimerLuxUpdate(unsigned long Lux);
	void onMainTimerTimeUpdate(uint16_t intTime);
	void onMainTimerChangeColor();
	void onMainTimerBlinkDot();

	unsigned long getLastLux() {return _lastLux;}
	
private:
	void _addModes();
	unsigned long _lastLux;
	uint8_t _currentBrightness;
	/// ���������� ��������, ���� �������� �� ����� ���������� �������, � �� ������� ������
	uint8_t _minBrightness;
	/// �������� ������������, ����� �������� ���� �� �������� ������������ ���� ������� � ����� �� ��������
	uint8_t _maxLuxValue;

	/// ��������� �������� ��� �������� ��������� �������, �� ����� �������� ����
	double _toBrightness;

	WorkModeInterface *mode;
	WorkModeInterface *defaultMode;

	bool _handMode;
};



#endif /* WORKMODE_H_ */