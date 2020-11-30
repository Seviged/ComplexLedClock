/*
 * LedClock.h
 *
 * Created: 24.03.2019 16:45:06
 *  Author: sevig
 */ 


#ifndef LEDCLOCK_H_
#define LEDCLOCK_H_

#include "light_ws2812.h"
#include "colorConverter.h"
#include "fireGenLib.h"

#define LEFT_LED_PIN    ((uint8_t)5)
#define RIGHT_LED_PIN   ((uint8_t)7)
#define LEDS            30

extern struct cRGB leftMass[LEDS];
extern struct cRGB rightMass[LEDS];

extern uint8_t leftMask[LEDS];
extern uint8_t rightMask[LEDS];


enum maskSide{
	LEFT = 0,
	RIGHT = 1
};

void clearMask(enum maskSide side, uint8_t pos);
void intToMaskLeft(int digit, uint8_t pos);
void intToMaskRight(int digit, uint8_t pos);
void fillAll(struct cRGB  *ledarray, uint8_t number_of_leds, struct cRGB color);
void fillByMask(struct cRGB  *ledarray, uint8_t *mask, uint16_t number_of_leds, struct cRGB color);
void fillByMaskHsv(struct cRGB  *ledarray, uint8_t *mask, uint16_t number_of_leds, hsv color, uint8_t leftFlag);
void fillBySingleColor(struct cRGB  *ledarray, uint8_t *mask, uint16_t number_of_leds, hsv color);
void fillByPallete(struct cRGB  *ledarray, uint8_t *mask, uint16_t number_of_leds, const CRGBPalette16 &pallete, uint8_t brightness);
void fillMaskByInt(int var);
void toggleDots();
void updateLeds();

void updateColorBrightness(hsv color);
void fillAndUpdateByPalleteWithDelay(const CRGBPalette16 &pallete, uint8_t delay, uint8_t brightness);
void fillAndUpdateBySingleColor(const hsv &color);



#endif /* LEDCLOCK_H_ */