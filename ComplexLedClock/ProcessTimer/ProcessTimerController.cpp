/*
 * ProcessTimerController.cpp
 *
 * Created: 08.05.2019 12:13:02
 *  Author: rzinich
 */ 

#include "ProcessTimerController.h"
#include "ProcessTimer.h"

 ProcessTimerController::ProcessTimerController()
{
    _timersCount = 0;
    _initHardwareTimer();
}

void ProcessTimerController::addTimer(ProcessTimer* timer)
{
    if(_timersCount < MAX_TIMERS)
    {
        _timers[_timersCount] = timer;
        ++_timersCount;
    }
}

void ProcessTimerController::processTimers()
{
    for(uint8_t i = 0; i < _timersCount; ++i)
    {
        _timers[i]->tick();
    }
}

void ProcessTimerController::enableCounter(bool value)
{
    if(value)
    {
        TCCR0B = _backupTimerRegister;
    }
    else
    {
        TCCR0B = 0;
        TCNT0 = 0;
    }
}

bool ProcessTimerController::isTimeReached(uint8_t number)
{
    for(uint8_t i = 0; i < _timersCount; ++i)
    {
        if (_timers[i]->number() == number)
        {
            return _timers[i]->isReached();
        }
    }
    
    return false;
}

void ProcessTimerController::_initHardwareTimer()
{
    TCCR0A = 0;
    TCCR0A |= (1 << WGM01);
    TCNT0 = 0;
    
    ///clk 8000000
    ///prescaler = clk/4
    ///interrupt time every 0.004992 sec or 4.99 msec
    _backupTimerRegister = 0;
    _backupTimerRegister |= (1 << CS02);
    OCR0A = 156;
    TIMSK0 |= (1 << OCIE0A);
}
