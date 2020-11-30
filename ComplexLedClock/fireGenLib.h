#ifndef FIREGENLIB_H_
#define FIREGENLIB_H_

#include "colorConverter.h"
#include "ModeDefines.h"

class CRGBPalette16;



cRGB getFireColor(int val, NoizeModeDefs defs);

uint8_t scale8_LEAVING_R1_DIRTY( uint8_t i, uint8_t scale);
uint8_t scale8( uint8_t i, uint8_t scale);
cRGB ColorFromPalette( const CRGBPalette16& pal, uint8_t index, uint8_t brightness);
uint8_t lsr(uint8_t div);
uint8_t inoise8(uint16_t x, uint16_t y);


class CRGBPalette16 {
	public:
	cRGB entries[16];
	CRGBPalette16() {};
	CRGBPalette16( const cRGB& c00,const cRGB& c01,const cRGB& c02,const cRGB& c03,
	const cRGB& c04,const cRGB& c05,const cRGB& c06,const cRGB& c07,
	const cRGB& c08,const cRGB& c09,const cRGB& c10,const cRGB& c11,
	const cRGB& c12,const cRGB& c13,const cRGB& c14,const cRGB& c15 )
	{
		entries[0]=c00; entries[1]=c01; entries[2]=c02; entries[3]=c03;
		entries[4]=c04; entries[5]=c05; entries[6]=c06; entries[7]=c07;
		entries[8]=c08; entries[9]=c09; entries[10]=c10; entries[11]=c11;
		entries[12]=c12; entries[13]=c13; entries[14]=c14; entries[15]=c15;
	};


	bool operator==( const CRGBPalette16 rhs)
	{
		const uint8_t* p = (const uint8_t*)(&(this->entries[0]));
		const uint8_t* q = (const uint8_t*)(&(rhs.entries[0]));
		if( p == q) return true;
		for( uint8_t i = 0; i < (sizeof( entries)); i++) {
			if( *p != *q) return false;
			p++;
			q++;
		}
		return true;
	}
	bool operator!=( const CRGBPalette16 rhs)
	{
		return !( *this == rhs);
	}
	
	inline cRGB& operator[] (uint8_t x) __attribute__((always_inline))
	{
		return entries[x];
	}
	inline const cRGB& operator[] (uint8_t x) const __attribute__((always_inline))
	{
		return entries[x];
	}

	inline cRGB& operator[] (int x) __attribute__((always_inline))
	{
		return entries[(uint8_t)x];
	}
	inline const cRGB& operator[] (int x) const __attribute__((always_inline))
	{
		return entries[(uint8_t)x];
	}

	operator cRGB*()
	{
		return &(entries[0]);
	}
};



#endif /* FIREGENLIB_H_ */