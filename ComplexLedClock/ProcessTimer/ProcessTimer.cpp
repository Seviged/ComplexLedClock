/*
 * ProcessTimer.cpp
 *
 * Created: 08.05.2019 12:15:35
 *  Author: rzinich
 */ 

#include "ProcessTimer.h"

ProcessTimer::ProcessTimer(uint8_t number, uint16_t period_ms) :
    _number(number)
{
    _defaultPeriod = period_ms / 5;
    if(period_ms < 5) _defaultPeriod = 0;
    reset();    
}

bool ProcessTimer::isReached()
{
    if (_currentPeriod == 0)
    {
        reset();
        return true;
    }
    return false;
}

void ProcessTimer::reset()
{
    _currentPeriod = _defaultPeriod;
}

void ProcessTimer::tick()
{
    if (_currentPeriod > 0)
    {
        --_currentPeriod;
    }
}
