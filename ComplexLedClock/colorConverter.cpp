/*
 * CFile1.c
 *
 * Created: 21.03.2019 10:17:45
 *  Author: sevig
 */ 


#include "colorConverter.h"
#include "light_ws2812.h"

void setRgb(doubleRgb *rgb, double rr, double gg, double bb)
{
	rgb->r = rr;
	rgb->g = gg;
	rgb->b = bb;
}

struct cRGB toCRGB(doubleRgb *rgb)
{
	struct cRGB color;
	color.r = rgb->r * 255;
	color.g = rgb->g * 255;
	color.b = rgb->b * 255;

	return color;
}

struct cRGB hsv2rgb(hsv HSV)
{
	doubleRgb RGB;
	double H = HSV.h, S = HSV.s, V = HSV.v,
	P, Q, T,
	fract;

	(H == 360.)?(H = 0.):(H /= 60.);
	fract = H - floor(H);

	P = V*(1. - S);
	Q = V*(1. - S*fract);
	T = V*(1. - S*(1. - fract));

	if      (0. <= H && H < 1.)
		setRgb(&RGB, V, T, P);
	else if (1. <= H && H < 2.)
		setRgb(&RGB, Q, V, P);
	else if (2. <= H && H < 3.)
		setRgb(&RGB, P, V, T);
	else if (3. <= H && H < 4.)
		setRgb(&RGB, P, Q, V);
	else if (4. <= H && H < 5.)
		setRgb(&RGB, T, P, V);
	else if (5. <= H && H < 6.)
		setRgb(&RGB, V, P, Q);
	else
		setRgb(&RGB, 0, 0, 0);

	return toCRGB(&RGB);
}

