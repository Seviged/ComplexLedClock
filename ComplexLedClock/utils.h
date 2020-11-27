/*
 * utils.h
 *
 * Created: 27.11.2020 23:31:32
 *  Author: Seviged
 */ 


#ifndef UTILS_H_
#define UTILS_H_

long map(long x, long in_min, long in_max, long out_min, long out_max);
double mapf(double val, double in_min, double in_max, double out_min, double out_max);
double constrainf(double x, double min, double max);
long constrain(long x, long min, long max);


#endif /* UTILS_H_ */