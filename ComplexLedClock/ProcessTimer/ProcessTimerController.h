/*
 * ProcessTimerController.h
 *
 * Created: 08.05.2019 12:12:55
 *  Author: rzinich
 */ 


#ifndef PROCESSTIMERCONTROLLER_H_
#define PROCESSTIMERCONTROLLER_H_

#include "ProcessTimer.h"
#define MAX_TIMERS 4

class ProcessTimerController
{
public:
    ProcessTimerController();
    void addTimer(ProcessTimer* timer);
    void processTimers();
    void enableCounter(bool);
    bool isTimeReached(uint8_t number); 
    
private:
    void _initHardwareTimer();
    
    ProcessTimer* _timers[MAX_TIMERS];
    uint8_t _timersCount;
    uint8_t _backupTimerRegister;
    
};




#endif /* PROCESSTIMERCONTROLLER_H_ */