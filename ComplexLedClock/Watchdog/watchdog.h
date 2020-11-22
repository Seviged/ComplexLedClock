/*
 * watchdog.h
 *
 * Created: 08.05.2019 11:58:43
 *  Author: rzinich
 */ 

#include <avr/wdt.h>

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

void WDT_off();
void WDT_run_2sec();
void WDT_reset();

#endif /* WATCHDOG_H_ */