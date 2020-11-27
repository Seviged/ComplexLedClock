#ifndef COLORCONVERTER_H_
#define COLORCONVERTER_H_

#include "light_ws2812.h"

#include <math.h>



typedef struct {
	double r;       // ? [0, 1]
	double g;       // ? [0, 1]
	double b;       // ? [0, 1]
} doubleRgb;

typedef struct {
	double h;       // ? [0, 360]
	double s;       // ? [0, 1]
	double v;       // ? [0, 1]
} hsv;

struct cRGB hsv2rgb(hsv HSV);
void setRgb(doubleRgb *rgb, double rr, double gg, double bb);
struct cRGB toCRGB(doubleRgb *rgb);


#endif /* COLORCONVERTER_H_ */