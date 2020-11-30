/*
 * ModeDefines.h
 *
 * Created: 29.11.2020 18:44:15
 *  Author: Seviged
 */ 


#ifndef MODEDEFINES_H_
#define MODEDEFINES_H_

#include "colorConverter.h"

enum ModeType{
	MODE_SIMPLE = 0,
	MODE_STATIC = 1,
	MODE_NOIZE  = 2
};


struct NoizeModeDefs{
	double hue_start;
	double hue_gap;
	double minBright;
	double maxBright;
	double minSat;
	double maxSat;
	uint8_t delay;
	bool correctBrightness = false;
};

struct SimpleModeDefs{
	hsv color;
};

struct StaticModeDefs{
	hsv color;
};

struct ModeData{
	NoizeModeDefs noizeDefs;
	SimpleModeDefs simpleDefs;
	StaticModeDefs staticDefs;
};

struct ModeDescription{
	ModeType type;
	ModeData data;
};




#endif /* MODEDEFINES_H_ */