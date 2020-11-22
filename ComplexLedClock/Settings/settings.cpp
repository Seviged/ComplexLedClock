#include "settings.h"
#include <avr/eeprom.h>

uint8_t minBrightness;
uint8_t maxLuxValue;

uint8_t readSettings(enum settings pos)
{
  return eeprom_read_byte((uint8_t*)pos);
}

void writeSettings(enum settings pos, uint8_t value)
{
  eeprom_write_byte((uint8_t*)pos, value);
}

void initSettings()
{
  if(readSettings(EE_MIN_BRIGHTNESS_INIT) == 0xFF)
  {
    writeSettings(EE_MIN_BRIGHTNESS, 22);
    writeSettings(EE_MIN_BRIGHTNESS_INIT, 1);
  }
  minBrightness = readSettings(EE_MIN_BRIGHTNESS);

  if(readSettings(EE_MAX_LUX_RANGE_INIT) == 0xFF)
  {
    writeSettings(EE_MAX_LUX_RANGE, 20);
    writeSettings(EE_MAX_LUX_RANGE_INIT, 1);
  }
  maxLuxValue = readSettings(EE_MAX_LUX_RANGE);
  
}
