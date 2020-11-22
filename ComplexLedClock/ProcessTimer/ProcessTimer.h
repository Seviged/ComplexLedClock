/*
 * ProcessTimer.h
 *
 * Created: 08.05.2019 12:15:45
 *  Author: rzinich
 */ 


#ifndef PROCESSTIMER_H_
#define PROCESSTIMER_H_

#include <avr/io.h>

class ProcessTimer
{
public:
    ProcessTimer(uint8_t number, uint16_t period_ms);
    uint8_t number() {return _number;}
    bool isReached();
    void reset();
    void tick();

private:
    uint8_t _number;
    uint16_t _defaultPeriod;
    uint16_t _currentPeriod;
    
};



#endif /* PROCESSTIMER_H_ */