/*
 * utils.cpp
 *
 * Created: 27.11.2020 23:31:43
 *  Author: Seviged
 */ 

#include "utils.h"

double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
	return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


long constrain(long x, long min, long max)
{
	if (x < min)
	{
		return min;
	}
	else if (x > max)
	{
		return max;
	}
	return x;
}


double constrainf(double x, double min, double max)
{
	if (x < min)
	{
		return min;
	}
	else if (x > max)
	{
		return max;
	}
	return x;
}