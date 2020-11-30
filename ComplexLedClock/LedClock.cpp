/*
 * LedClock.c
 *
 * Created: 24.03.2019 16:45:18
 *  Author: sevig
 */ 

#include "LedClock.h"
#include "fireGenLib.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

char dotStash = 0;

struct cRGB leftMass[LEDS];
struct cRGB rightMass[LEDS];

double dotValue = 1.;
uint8_t dotVector = 1;

uint8_t leftMask[LEDS];
uint8_t rightMask[LEDS];

hsv left;
hsv right;

int counter = 13531;



void clearMask(enum maskSide side, uint8_t pos)
{
	if(side == LEFT)
	{
		for(int i = 0; i < 14; ++i)
		{
			leftMask[1 + i + 1 + 14 * pos] = 0;
		}
	}
	else
	{
		for(int i = 0; i < 14; ++i)
		{
			rightMask[1 + i + 1 + 14 * pos] = 0;
		}
	}
}

void intToMaskLeft(int digit, uint8_t pos)
{
	clearMask(LEFT, pos);
	switch(digit)
	{
		case 1:
		leftMask[1 + 1 + 14 * pos] = 1;
		leftMask[2 + 1 + 14 * pos] = 1;
		leftMask[11 + 1 + 14 * pos] = 1;
		leftMask[12 + 1 + 14 * pos] = 1;
		break;
		case 2:
		leftMask[9 + 1 + 14 * pos] = 1;
		leftMask[10 + 1 + 14 * pos] = 1;
		leftMask[11 + 1 + 14 * pos] = 1;
		leftMask[12 + 1 + 14 * pos] = 1;
		leftMask[13 + 1 + 14 * pos] = 1;
		leftMask[14 + 1 + 14 * pos] = 1;
		leftMask[6 + 1 + 14 * pos] = 1;
		leftMask[5 + 1 + 14 * pos] = 1;
		leftMask[4 + 1 + 14 * pos] = 1;
		leftMask[3 + 1 + 14 * pos] = 1;
		break;
		case 3:
		leftMask[9 + 1 + 14 * pos] = 1;
		leftMask[10 + 1 + 14 * pos] = 1;
		leftMask[11 + 1 + 14 * pos] = 1;
		leftMask[12 + 1 + 14 * pos] = 1;
		leftMask[13 + 1 + 14 * pos] = 1;
		leftMask[14 + 1 + 14 * pos] = 1;
		leftMask[1 + 1 + 14 * pos] = 1;
		leftMask[2 + 1 + 14 * pos] = 1;
		leftMask[4 + 1 + 14 * pos] = 1;
		leftMask[3 + 1 + 14 * pos] = 1;
		break;
		case 4:
		leftMask[8 + 1 + 14 * pos] = 1;
		leftMask[7 + 1 + 14 * pos] = 1;
		leftMask[11 + 1 + 14 * pos] = 1;
		leftMask[12 + 1 + 14 * pos] = 1;
		leftMask[13 + 1 + 14 * pos] = 1;
		leftMask[14 + 1 + 14 * pos] = 1;
		leftMask[1 + 1 + 14 * pos] = 1;
		leftMask[2 + 1 + 14 * pos] = 1;
		break;
		case 5:
		leftMask[9 + 1 + 14 * pos] = 1;
		leftMask[10 + 1 + 14 * pos] = 1;
		leftMask[8 + 1 + 14 * pos] = 1;
		leftMask[7 + 1 + 14 * pos] = 1;
		leftMask[13 + 1 + 14 * pos] = 1;
		leftMask[14 + 1 + 14 * pos] = 1;
		leftMask[1 + 1 + 14 * pos] = 1;
		leftMask[2 + 1 + 14 * pos] = 1;
		leftMask[4 + 1 + 14 * pos] = 1;
		leftMask[3 + 1 + 14 * pos] = 1;
		break;
		case 6:
		leftMask[9 + 1 + 14 * pos] = 1;
		leftMask[10 + 1 + 14 * pos] = 1;
		leftMask[8 + 1 + 14 * pos] = 1;
		leftMask[7 + 1 + 14 * pos] = 1;
		leftMask[13 + 1 + 14 * pos] = 1;
		leftMask[14 + 1 + 14 * pos] = 1;
		leftMask[6 + 1 + 14 * pos] = 1;
		leftMask[5 + 1 + 14 * pos] = 1;
		leftMask[4 + 1 + 14 * pos] = 1;
		leftMask[3 + 1 + 14 * pos] = 1;
		leftMask[1 + 1 + 14 * pos] = 1;
		leftMask[2 + 1 + 14 * pos] = 1;
		break;
		case 7:
		leftMask[1 + 1 + 14 * pos] = 1;
		leftMask[2 + 1 + 14 * pos] = 1;
		leftMask[11 + 1 + 14 * pos] = 1;
		leftMask[12 + 1 + 14 * pos] = 1;
		leftMask[9 + 1 + 14 * pos] = 1;
		leftMask[10 + 1 + 14 * pos] = 1;
		break;
		case 8:
		leftMask[9 + 1 + 14 * pos] = 1;
		leftMask[10 + 1 + 14 * pos] = 1;
		leftMask[8 + 1 + 14 * pos] = 1;
		leftMask[7 + 1 + 14 * pos] = 1;
		leftMask[13 + 1 + 14 * pos] = 1;
		leftMask[14 + 1 + 14 * pos] = 1;
		leftMask[6 + 1 + 14 * pos] = 1;
		leftMask[5 + 1 + 14 * pos] = 1;
		leftMask[4 + 1 + 14 * pos] = 1;
		leftMask[3 + 1 + 14 * pos] = 1;
		leftMask[1 + 1 + 14 * pos] = 1;
		leftMask[2 + 1 + 14 * pos] = 1;
		leftMask[11 + 1 + 14 * pos] = 1;
		leftMask[12 + 1 + 14 * pos] = 1;
		break;
		case 9:
		leftMask[9 + 1 + 14 * pos] = 1;
		leftMask[10 + 1 + 14 * pos] = 1;
		leftMask[8 + 1 + 14 * pos] = 1;
		leftMask[7 + 1 + 14 * pos] = 1;
		leftMask[13 + 1 + 14 * pos] = 1;
		leftMask[14 + 1 + 14 * pos] = 1;
		leftMask[4 + 1 + 14 * pos] = 1;
		leftMask[3 + 1 + 14 * pos] = 1;
		leftMask[1 + 1 + 14 * pos] = 1;
		leftMask[2 + 1 + 14 * pos] = 1;
		leftMask[11 + 1 + 14 * pos] = 1;
		leftMask[12 + 1 + 14 * pos] = 1;
		break;
		case 0:
		leftMask[9 + 1 + 14 * pos] = 1;
		leftMask[10 + 1 + 14 * pos] = 1;
		leftMask[8 + 1 + 14 * pos] = 1;
		leftMask[7 + 1 + 14 * pos] = 1;
		leftMask[6 + 1 + 14 * pos] = 1;
		leftMask[5 + 1 + 14 * pos] = 1;
		leftMask[4 + 1 + 14 * pos] = 1;
		leftMask[3 + 1 + 14 * pos] = 1;
		leftMask[1 + 1 + 14 * pos] = 1;
		leftMask[2 + 1 + 14 * pos] = 1;
		leftMask[11 + 1 + 14 * pos] = 1;
		leftMask[12 + 1 + 14 * pos] = 1;
		break;
		default:
		break;
	}
}
void intToMaskRight(int digit, uint8_t pos)
{
	clearMask(RIGHT, pos);
	switch(digit)
	{
		case 1:
		rightMask[4 + 1 + 14 * pos] = 1;
		rightMask[3 + 1 + 14 * pos] = 1;
		rightMask[14 + 1 + 14 * pos] = 1;
		rightMask[13 + 1 + 14 * pos] = 1;
		break;
		case 2:
		rightMask[4 + 1 + 14 * pos] = 1;
		rightMask[3 + 1 + 14 * pos] = 1;
		rightMask[6 + 1 + 14 * pos] = 1;
		rightMask[5 + 1 + 14 * pos] = 1;
		rightMask[1 + 1 + 14 * pos] = 1;
		rightMask[2 + 1 + 14 * pos] = 1;
		rightMask[9 + 1 + 14 * pos] = 1;
		rightMask[10 + 1 + 14 * pos] = 1;
		rightMask[11 + 1 + 14 * pos] = 1;
		rightMask[12 + 1 + 14 * pos] = 1;
		break;
		case 3:
		rightMask[4 + 1 + 14 * pos] = 1;
		rightMask[3 + 1 + 14 * pos] = 1;
		rightMask[6 + 1 + 14 * pos] = 1;
		rightMask[5 + 1 + 14 * pos] = 1;
		rightMask[1 + 1 + 14 * pos] = 1;
		rightMask[2 + 1 + 14 * pos] = 1;
		rightMask[13 + 1 + 14 * pos] = 1;
		rightMask[14 + 1 + 14 * pos] = 1;
		rightMask[11 + 1 + 14 * pos] = 1;
		rightMask[12 + 1 + 14 * pos] = 1;
		break;
		case 4:
		rightMask[4 + 1 + 14 * pos] = 1;
		rightMask[3 + 1 + 14 * pos] = 1;
		rightMask[7 + 1 + 14 * pos] = 1;
		rightMask[8 + 1 + 14 * pos] = 1;
		rightMask[1 + 1 + 14 * pos] = 1;
		rightMask[2 + 1 + 14 * pos] = 1;
		rightMask[13 + 1 + 14 * pos] = 1;
		rightMask[14 + 1 + 14 * pos] = 1;
		break;
		case 5:
		rightMask[7 + 1 + 14 * pos] = 1;
		rightMask[8 + 1 + 14 * pos] = 1;
		rightMask[6 + 1 + 14 * pos] = 1;
		rightMask[5 + 1 + 14 * pos] = 1;
		rightMask[1 + 1 + 14 * pos] = 1;
		rightMask[2 + 1 + 14 * pos] = 1;
		rightMask[14 + 1 + 14 * pos] = 1;
		rightMask[13 + 1 + 14 * pos] = 1;
		rightMask[11 + 1 + 14 * pos] = 1;
		rightMask[12 + 1 + 14 * pos] = 1;
		break;
		case 6:
		rightMask[7 + 1 + 14 * pos] = 1;
		rightMask[8 + 1 + 14 * pos] = 1;
		rightMask[6 + 1 + 14 * pos] = 1;
		rightMask[5 + 1 + 14 * pos] = 1;
		rightMask[1 + 1 + 14 * pos] = 1;
		rightMask[2 + 1 + 14 * pos] = 1;
		rightMask[14 + 1 + 14 * pos] = 1;
		rightMask[13 + 1 + 14 * pos] = 1;
		rightMask[11 + 1 + 14 * pos] = 1;
		rightMask[12 + 1 + 14 * pos] = 1;
		rightMask[9 + 1 + 14 * pos] = 1;
		rightMask[10 + 1 + 14 * pos] = 1;
		break;
		case 7:
		rightMask[4 + 1 + 14 * pos] = 1;
		rightMask[3 + 1 + 14 * pos] = 1;
		rightMask[14 + 1 + 14 * pos] = 1;
		rightMask[13 + 1 + 14 * pos] = 1;
		rightMask[6 + 1 + 14 * pos] = 1;
		rightMask[5 + 1 + 14 * pos] = 1;
		break;
		case 8:
		rightMask[7 + 1 + 14 * pos] = 1;
		rightMask[8 + 1 + 14 * pos] = 1;
		rightMask[6 + 1 + 14 * pos] = 1;
		rightMask[5 + 1 + 14 * pos] = 1;
		rightMask[1 + 1 + 14 * pos] = 1;
		rightMask[2 + 1 + 14 * pos] = 1;
		rightMask[14 + 1 + 14 * pos] = 1;
		rightMask[13 + 1 + 14 * pos] = 1;
		rightMask[11 + 1 + 14 * pos] = 1;
		rightMask[12 + 1 + 14 * pos] = 1;
		rightMask[9 + 1 + 14 * pos] = 1;
		rightMask[10 + 1 + 14 * pos] = 1;
		rightMask[3 + 1 + 14 * pos] = 1;
		rightMask[4 + 1 + 14 * pos] = 1;
		break;
		case 9:
		rightMask[7 + 1 + 14 * pos] = 1;
		rightMask[8 + 1 + 14 * pos] = 1;
		rightMask[6 + 1 + 14 * pos] = 1;
		rightMask[5 + 1 + 14 * pos] = 1;
		rightMask[1 + 1 + 14 * pos] = 1;
		rightMask[2 + 1 + 14 * pos] = 1;
		rightMask[14 + 1 + 14 * pos] = 1;
		rightMask[13 + 1 + 14 * pos] = 1;
		rightMask[11 + 1 + 14 * pos] = 1;
		rightMask[12 + 1 + 14 * pos] = 1;
		rightMask[3 + 1 + 14 * pos] = 1;
		rightMask[4 + 1 + 14 * pos] = 1;
		break;
		case 0:
		rightMask[7 + 1 + 14 * pos] = 1;
		rightMask[8 + 1 + 14 * pos] = 1;
		rightMask[6 + 1 + 14 * pos] = 1;
		rightMask[5 + 1 + 14 * pos] = 1;
		rightMask[14 + 1 + 14 * pos] = 1;
		rightMask[13 + 1 + 14 * pos] = 1;
		rightMask[11 + 1 + 14 * pos] = 1;
		rightMask[12 + 1 + 14 * pos] = 1;
		rightMask[9 + 1 + 14 * pos] = 1;
		rightMask[10 + 1 + 14 * pos] = 1;
		rightMask[3 + 1 + 14 * pos] = 1;
		rightMask[4 + 1 + 14 * pos] = 1;
		break;
		default:
		break;
	}
}

void fillByMaskHsv(struct cRGB  *ledarray, uint8_t *mask, uint16_t number_of_leds, hsv color, uint8_t leftFlag)
{
	struct cRGB blank;
	blank.r = 0;
	blank.g = 0;
	blank.b = 0;

	for(uint8_t i = 0; i < number_of_leds; ++i)
	{
        hsv hcolor = color;
        if(i < 2)
        {
            if(leftFlag == 1)
            {
                hcolor.h = left.h;
                left.h = left.h + 1;
                if (left.h > 360) left.h = 0;
            }
            else
            {
                hcolor.h = right.h;
                right.h = right.h - 1;
                if (right.h < 0) right.h = 360;
            }
        }   
                 
		struct cRGB clr = hsv2rgb(hcolor);
		if(mask[i] == 0)
		clr = blank;

		ledarray[i] = clr;
	}
}


void fillByPallete(struct cRGB  *ledarray, uint8_t *mask, uint16_t number_of_leds, const CRGBPalette16 &pallete, uint8_t brightness)
{
	struct cRGB blank;
	blank.r = 0;
	blank.g = 0;
	blank.b = 0;

	for(uint8_t i = 0; i < number_of_leds; ++i)
	{
		struct cRGB clr = blank;
		if(mask[i] != 0)
		{
			clr = ColorFromPalette(pallete, (inoise8(i * 15, counter)), brightness);
			counter += 1;
		}

		ledarray[i] = clr;
	}
}

void fillBySingleColor(struct cRGB  *ledarray, uint8_t *mask, uint16_t number_of_leds, hsv color)
{
	struct cRGB blank;
	blank.r = 0;
	blank.g = 0;
	blank.b = 0;

	for(uint8_t i = 0; i < number_of_leds; ++i)
	{
		struct cRGB clr = blank;
		if(mask[i] != 0)
		{
			clr = hsv2rgb(color);
		}

		ledarray[i] = clr;
	}
}

void fillMaskByInt(int var)
{
	intToMaskRight(var % 10, 1);
	var /= 10;
	intToMaskRight(var % 10, 0);
	var /= 10;
	intToMaskLeft(var % 10, 0);
	var /= 10;
    if(var == 0) var = 11;
	intToMaskLeft(var, 1);
}

void toggleDots()
{
    rightMask[0] = 1;
    rightMask[1] = 1;
    leftMask[0] = 1;
    leftMask[1] = 1;
    
    left.h = 0;
    right.h = 0;
}

void updateLeds()
{
    //тут не правильно, надо задать номер порта а не номер на плате!
    //порт Б по умолчанию...
     ws2812_setleds_pin(leftMass, LEDS, _BV(LEFT_LED_PIN));
     ws2812_setleds_pin(rightMass, LEDS, _BV(RIGHT_LED_PIN));
}

void updateColorBrightness(hsv color)
{
    fillByMaskHsv(leftMass, leftMask, LEDS, color, 1);
    fillByMaskHsv(rightMass, rightMask, LEDS, color, 0);
    
    updateLeds();
}

void fillAndUpdateByPalleteWithDelay(const CRGBPalette16 &pallete, uint8_t delay, uint8_t brightness)
{
	fillByPallete(leftMass, leftMask, LEDS, pallete, brightness);
	fillByPallete(rightMass, rightMask, LEDS, pallete, brightness);
	
	updateLeds();
	for(uint8_t i = 0; i < delay; ++i)
	{
		_delay_ms(1);
	}
}

void fillAndUpdateBySingleColor(const hsv &color)
{
	fillBySingleColor(leftMass, leftMask, LEDS, color);
	fillBySingleColor(rightMass, rightMask, LEDS, color);
	
	updateLeds();
}
