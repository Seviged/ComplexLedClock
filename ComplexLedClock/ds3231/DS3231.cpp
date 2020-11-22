/*
 * DS3231.cpp
 *
 * Created: 16.05.2019 17:03:24
 *  Author: rzinich
 */ 

#include "DS3231.h"
#include "../i2c/i2c_master.h"

 RealTimeClock::RealTimeClock()
 {
     setHours(0);
     setMinutes(0);
     setMonth(0);
     setSeconds(0);
     setYear(0);
     setDay(0);
     setDayOfWeek(0);
 }

uint8_t RealTimeClock::DS3231_setTime(uint8_t second, uint8_t minute,uint8_t hour,
                       uint8_t dayOfWeek,
                       uint8_t dayOfMonth,
                       uint8_t month,
                       uint8_t year)
{
    uint8_t result = 0;
    
    result += i2c_start(DS3231_ADDRESS, I2C_WRITE);
    result += i2c_write(0x00); //move pointer location 0 which is seconds
    result += i2c_write(decimalToBcd(second));
    result += i2c_write(decimalToBcd(minute));
    result += i2c_write(decimalToBcd(hour));
    result += i2c_write(decimalToBcd(dayOfWeek));
    result += i2c_write(decimalToBcd(dayOfMonth));
    result += i2c_write(decimalToBcd(month));
    result += i2c_write(decimalToBcd(year));
    
    i2c_stop();
    
    if (result)
    {
        return 1;
    }
    
    return 0;
}

uint8_t RealTimeClock::DS3231_getTime(uint8_t *second, uint8_t *minute,
                       uint8_t *hour,
                       uint8_t *dayOfWeek,
                       uint8_t *dayOfMonth,
                       uint8_t *month,
                       uint8_t *year)
{
    
    uint8_t result = 0;
    
    result += i2c_start(DS3231_ADDRESS, I2C_WRITE);
    result += i2c_write(0x00); //move pointer location 0 which is seconds
    i2c_stop();
    
    if(result) return 1;
    
    result += i2c_start(DS3231_ADDRESS, I2C_READ);
    uint8_t data;
    
    result += i2c_read_ack(&data);
    if(result) return 1;
    if (second != NULL)
        *second     = bcdToDec(data & 0x7f);
    
    result += i2c_read_ack(&data);
    if(result) return 1;
    if (minute != NULL)
    *minute     = bcdToDec(data);
    
    result += i2c_read_ack(&data);
    if(result) return 1;
    if (hour != NULL)
    *hour       = bcdToDec(data & 0x3f);
    
    result += i2c_read_ack(&data);
    if(result) return 1;
    if (dayOfWeek != NULL)
    *dayOfWeek  = bcdToDec(data);
    
    result += i2c_read_ack(&data);
    if(result) return 1;
    if (dayOfMonth != NULL)
    *dayOfMonth = bcdToDec(data);
    
    result += i2c_read_ack(&data);
    if(result) return 1;
    if (month != NULL)
    *month      = bcdToDec(data);
    
    result += i2c_read_nack(&data);
    if(result) return 1;
    if (year != NULL)
    *year       = bcdToDec(data);
    
    return 0;
}


void RealTimeClock::setTimeToRtc(uint8_t hour, uint8_t min, uint8_t sec)
{
    setHours(hour);
    setMinutes(min);
    setSeconds(sec);
    
    DS3231_setTime(_second, _minute, _hour, _dayOfWeek, _dayOfMonth, _month, _year);
}

void RealTimeClock::updateTimeFromRtc()
{
    DS3231_getTime(&_second, &_minute,  &_hour, &_dayOfWeek, &_dayOfMonth, &_month, &_year);
}
