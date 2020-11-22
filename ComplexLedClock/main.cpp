
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Watchdog/watchdog.h"
#include "LedClock.h"
#include "ProcessTimer/ProcessTimerController.h"
#include "ProcessTimer/ProcessTimer.h"
#include "i2c_master.h"
#include "i2c/i2c_master.h"
#include "ds3231/DS3231.h"
#include "nrf24/network.h"
#include "Settings/settings.h"
#include "luxMeter/TSL2581.h"

RealTimeClock rtc;
ProcessTimerController timerController;
WaveShare_TSL2581 tsl = WaveShare_TSL2581();

#define LUXLEN 10
uint8_t luxFlag = -1;
uint8_t isFirst = 1;
unsigned long luxMass[LUXLEN];

uint8_t nomLux()
{
    uint16_t sumLux = 0;
    for(int i = 0; i < LUXLEN; ++i)
    {
        sumLux += luxMass[i];
    }
    
    if (isFirst) return 255;
    return sumLux / LUXLEN;
}

void addLux(uint8_t newLux)
{
    luxFlag++;
    if (luxFlag == LUXLEN)
    {
        luxFlag = 0;
        isFirst = 0;
    }
    
    luxMass[luxFlag] = newLux;
}

enum Timers{
    TIMER_DISPLAY_UPDATE,
    TIMER_LUX_UPDATE,
    TIMER_TIME_UPDATE,
    TIMER_CHANGE_COLOR,
    TIMER_BLINK_DOT
    };
    
ISR (TIMER0_COMPA_vect)
{
    timerController.processTimers(); 
}

hsv mainColor;
uint8_t currentBrightness = 0xff;
double toBrightness;


void updateTime()
{
    uint16_t currentTime = 0;
    rtc.updateTimeFromRtc();
    currentTime = (rtc.getHours() * 100) + rtc.getMinutes();
    fillMaskByInt(currentTime);
}


int main(void)
{
    WDT_off();
    
    mainColor.h = 0;
    mainColor.s = 1;
    mainColor.v = 1;
    toBrightness = 1;
    
    toggleDots();
    
    ProcessTimer luxTimer(TIMER_LUX_UPDATE, 500);
    ProcessTimer timeTimer(TIMER_TIME_UPDATE, 200);
    ProcessTimer changeColor(TIMER_CHANGE_COLOR, 5000);
    ProcessTimer blinkDot(TIMER_BLINK_DOT, 5);
    
    timerController.addTimer(&luxTimer);
    timerController.addTimer(&timeTimer);
    timerController.addTimer(&changeColor);
    timerController.addTimer(&blinkDot);
   
    tsl.TSL2581_power_on();
    _delay_ms(200);
    tsl.TSL2581_config();
    
    initSettings();
    
    i2c_init();
    
    updateTime();
    
    initNetwork(); 
    
    timerController.enableCounter(true);
    
    sei();   
    
    WDT_run_2sec();  
    while (1) 
    {
        processNetwork();
        
        WDT_reset();
                    
        if(timerController.isTimeReached(TIMER_LUX_UPDATE))
        {
            unsigned long Lux;
            tsl.TSL2581_Read_Channel();
            Lux = tsl.calculateLux(2, NOM_INTEG_CYCLE);
            
            if(Lux < maxLuxValue && minBrightness + Lux * 11 <= 255)
            {
                currentBrightness = minBrightness + Lux * 11;
            }
            else currentBrightness = 0xff;
            
            addLux(currentBrightness);
            
            WDT_reset();
        }
        if(timerController.isTimeReached(TIMER_TIME_UPDATE))
        {
            updateTime();
        }
        if(timerController.isTimeReached(TIMER_CHANGE_COLOR))
        {
            mainColor.h++;
            if (mainColor.h > 360)
            {
                mainColor.h = 0;
            }
        }
        if(timerController.isTimeReached(TIMER_BLINK_DOT))
        {
            double value = (double)nomLux() / 0xFF;
            
            const double pitch = 0.01;
            if (toBrightness > value - pitch)
            {
                toBrightness -= pitch;
            }
            else if(toBrightness < value + pitch)
            {
                toBrightness += pitch;
            }
            else
            {
                toBrightness = value;
            }
            if(toBrightness < 0.2) toBrightness = 0.2;
            
            mainColor.v = toBrightness;
            
            updateColorBrightness(mainColor);
        }
        WDT_reset();
    }
}

