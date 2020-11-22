/*
 * watchdog.cpp
 *
 * Created: 08.05.2019 11:59:32
 *  Author: rzinich
 */ 

#include "watchdog.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void WDT_off()
{
    cli();
    wdt_reset();
    /* Clear WDRF in MCUSR */
    MCUSR &= ~(1<<WDRF);
    /* Write logical one to WDCE and WDE */
    /* Keep old prescaler setting to prevent unintentional time-out */
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    /* Turn off WDT */
    WDTCSR = 0x00;
    sei();
}

void WDT_run_2sec()
{
    cli();
    wdt_reset();
    WDTCSR = WDTCSR | (1<<WDCE) | (1<<WDE);
    // запуск сторожевого таймера с предделителем (2sec)
    WDTCSR = (1<<WDE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
    sei();      // разрешить прерывания
}

void WDT_reset()
{
    wdt_reset();
}
