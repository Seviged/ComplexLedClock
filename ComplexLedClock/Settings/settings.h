#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <avr/io.h>

extern uint8_t minBrightness;
extern uint8_t maxLuxValue;

enum settings{
  EE_MIN_BRIGHTNESS_INIT = 0,
  EE_MIN_BRIGHTNESS = 1,
  EE_MAX_LUX_RANGE_INIT = 2,
  EE_MAX_LUX_RANGE = 3
};

uint8_t readSettings(enum settings pos);
void writeSettings(enum settings pos, uint8_t value);
void initSettings();


#endif //_SETTINGS_H_
