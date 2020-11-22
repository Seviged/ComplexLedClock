//
//  NullWolf DS3231.h
//  RTC, EEPROM, TEMP, IIC test
//
//  Created by NullWolf on 06/05/16.
//  
//

#ifndef DS3231_h
#define DS3231_h


#include <stdio.h>
#include <avr/io.h>


#define  DS3231_ADDRESS      0x68
#define  DS3231_REG_SEC		 0x00
#define  DS3231_REG_MIN		 0x01
#define  DS3231_REG_HOUR     0x02
#define  DS3231_REG_DOW		 0x03
#define  DS3231_REG_DATE     0x04
#define  DS3231_REG_MON		 0x05
#define  DS3231_REG_YEAR     0x06
#define  DS3231_REG_CON		 0x0E
#define  DS3231_REG_STATUS	 0x0F
#define  DS3231_REG_AGING	 0x10
#define  DS3231_REG_TEMPM	 0x11
#define  DS3231_REG_TEMPL	 0x12


class RealTimeClock
{
    
public:
    RealTimeClock();
    
    void setTimeToRtc(uint8_t hour, uint8_t min, uint8_t sec);
    void updateTimeFromRtc();
    
    void setSeconds(uint8_t var) {_second = var;}
    uint8_t getSeconds() {return _second;}
        
    void setMinutes(uint8_t var) {_minute = var;}
    uint8_t getMinutes() {return _minute;}
        
    void setHours(uint8_t var) {_hour = var;}
    uint8_t getHours() {return _hour;}
        
    void setDayOfWeek(uint8_t var) {_dayOfWeek = var;}
    uint8_t getDayOfWeek() {return _dayOfWeek;}
        
    void setDay(uint8_t var) {_dayOfMonth = var;}
    uint8_t getDay() {return _dayOfMonth;}
        
    void setMonth(uint8_t var) {_month = var;}
    uint8_t getMonth() {return _month;}
        
    void setYear(uint8_t var) {_year = var;}
    uint8_t getYear() {return _year;}
        
    
private:
    uint8_t _second;
    uint8_t _minute;
    uint8_t _hour;
    uint8_t _dayOfWeek;
    uint8_t _dayOfMonth;
    uint8_t _month;
    uint8_t _year;
      
    // Convert normal decimal numbers to binary coded decimal
    uint8_t decimalToBcd(uint8_t val){
        return( (val/10*16) + (val%10) );
    }

    // Convert binary coded decimal to normal decimal numbers
    uint8_t bcdToDec(uint8_t val){
        return( (val/16*10) + (val%16) );
    }

    uint8_t DS3231_setTime(uint8_t second, uint8_t minute,uint8_t hour, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint8_t year);
    uint8_t DS3231_getTime(uint8_t *second, uint8_t *minute, uint8_t *hour, uint8_t *dayOfWeek, uint8_t *dayOfMonth, uint8_t *month, uint8_t *year);

};

#endif /* DS3231_h */

