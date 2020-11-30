/*
 * fireGenLib.cpp
 *
 * Created: 27.11.2020 23:25:09
 *  Author: Seviged
 */ 

#include "fireGenLib.h"
#include "utils.h"
#include <avr/pgmspace.h>


// Note: only the 'near' memory wrappers are provided.
// If you're using 'far' memory, you already have
// portability issues to work through, but you could
// add more support here if needed.
#define FL_PGM_READ_BYTE_NEAR(x)  (pgm_read_byte_near(x))
#define P(x) FL_PGM_READ_BYTE_NEAR(p + x)
#define EASE8(x)  (ease8InOutQuad(x) )

PROGMEM static uint8_t const p[] = { 151,160,137,91,90,15,
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,151
};


//// настройки пламени
//#define HUE_GAP 360//45.0      // заброс по hue
////#define FIRE_STEP 25    // шаг изменени€ "€зыков" пламени
//#define HUE_START 0// 15     // начальный цвет огн€ (0 красный, 80 зелЄный, 140 молни€, 190 розовый)
////#define HUE_COEF 0.7    // коэффициент цвета огн€ (чем больше - тем дальше заброс по цвету)
////#define SMOOTH_K 0.15   // коэффициент плавности огн€
//#define MIN_BRIGHT 0.5 //50   // мин. €ркость огн€
//#define MAX_BRIGHT 1.0  //240  // макс. €ркость огн€
//#define MIN_SAT 0.90     //180     // мин. насыщенность
//#define MAX_SAT 1       //240     // макс. насыщенность

cRGB getFireColor(int val, NoizeModeDefs defs) 
{
	hsv col;
	col.h = defs.hue_start + mapf(val, 0, 255, 0, defs.hue_gap);
	col.s = constrainf(mapf(val, 0, 255, defs.maxSat, defs.minSat), 0, 1);
	col.v = constrainf(mapf(val, 0, 255, defs.minBright, defs.maxBright), 0, 1);
	return hsv2rgb(col);
}


uint8_t lsr(uint8_t div)
{
	#if defined(__AVR__)
	div /= 2;
	div /= 2;
	div /= 2;
	div /= 2;
	#else
	div >>= 4;
	#endif
	return div;
}


uint8_t scale8_LEAVING_R1_DIRTY( uint8_t i, uint8_t scale)
{
	//fixed return (((uint16_t)i) * ((uint16_t)(scale)+1)) >> 8;
	return ((int)i * (int)(scale) ) >> 8;
}


uint8_t scale8( uint8_t i, uint8_t scale)
{
	return ((uint16_t)i * (uint16_t)(scale) ) >> 8;
}


int8_t lerp7by8( int8_t a, int8_t b, uint8_t frac)
{
	int8_t result;
	if( b > a) {
		uint8_t delta = b - a;
		uint8_t scaled = scale8( delta, frac);
		result = a + scaled;
		} else {
		uint8_t delta = a - b;
		uint8_t scaled = scale8( delta, frac);
		result = a - scaled;
	}
	return result;
}


int8_t avg7( int8_t i, int8_t j)
{
	return ((i + j) >> 1) + (i & 0x1);
}


int8_t grad8(uint8_t hash, int8_t x, int8_t y)
{
	// since the tests below can be done bit-wise on the bottom
	// three bits, there's no need to mask off the higher bits
	//  hash = hash & 7;

	int8_t u,v;
	if( hash & 4) {
		u = y; v = x;
		} else {
		u = x; v = y;
	}

	if(hash&1) { u = -u; }
	if(hash&2) { v = -v; }

	return avg7(u,v);
}


cRGB ColorFromPalette( const CRGBPalette16& pal, uint8_t index, uint8_t brightness)
{
	//      hi4 = index >> 4;
	uint8_t hi4 = lsr(index);
	uint8_t lo4 = index & 0x0F;
	
	// const CRGB* entry = &(pal[0]) + hi4;
	// since hi4 is always 0..15, hi4 * sizeof(CRGB) can be a single-byte value,
	// instead of the two byte 'int' that avr-gcc defaults to.
	// So, we multiply hi4 X sizeof(CRGB), giving hi4XsizeofCRGB;
	uint8_t hi4XsizeofCRGB = hi4 * sizeof(cRGB);
	// We then add that to a base array pointer.
	const cRGB* entry = (cRGB*)( (uint8_t*)(&(pal[0])) + hi4XsizeofCRGB);
	
	uint8_t blend = lo4 && true;
	
	uint8_t red1   = entry->r;
	uint8_t green1 = entry->g;
	uint8_t blue1  = entry->b;
	
	
	if( blend ) {
		
		if( hi4 == 15 ) {
			entry = &(pal[0]);
			} else {
			entry++;
		}
		
		uint8_t f2 = lo4 << 4;
		uint8_t f1 = 255 - f2;
		
		//    rgb1.nscale8(f1);
		uint8_t red2   = entry->r;
		red1   = scale8_LEAVING_R1_DIRTY( red1,   f1);
		red2   = scale8_LEAVING_R1_DIRTY( red2,   f2);
		red1   += red2;

		uint8_t green2 = entry->g;
		green1 = scale8_LEAVING_R1_DIRTY( green1, f1);
		green2 = scale8_LEAVING_R1_DIRTY( green2, f2);
		green1 += green2;

		uint8_t blue2  = entry->b;
		blue1  = scale8_LEAVING_R1_DIRTY( blue1,  f1);
		blue2  = scale8_LEAVING_R1_DIRTY( blue2,  f2);
		blue1  += blue2;
		
		//cleanup_R1();
	}
	
	if( brightness != 255) {
		if( brightness ) {
			brightness++; // adjust for rounding
			// Now, since brightness is nonzero, we don't need the full scale8_video logic;
			// we can just to scale8 and then add one (unless scale8 fixed) to all nonzero inputs.
			if( red1 )   {
				red1 = scale8_LEAVING_R1_DIRTY( red1, brightness);
				//#if !(FASTLED_SCALE8_FIXED==1)
				red1++;
				//#endif
			}
			if( green1 ) {
				green1 = scale8_LEAVING_R1_DIRTY( green1, brightness);
				//#if !(FASTLED_SCALE8_FIXED==1)
				green1++;
				//#endif
			}
			if( blue1 )  {
				blue1 = scale8_LEAVING_R1_DIRTY( blue1, brightness);
				//#if !(FASTLED_SCALE8_FIXED==1)
				blue1++;
				//#endif
			}
			//cleanup_R1();
			} else {
			red1 = 0;
			green1 = 0;
			blue1 = 0;
		}
	}
	
	cRGB colorS;
	colorS.r = red1;
	colorS.b = blue1;
	colorS.g = green1;
	return colorS;
}


uint8_t ease8InOutQuad( uint8_t i)
{
	uint8_t j = i;
	if( j & 0x80 ) {
		j = 255 - j;
	}
	uint8_t jj  = scale8(  j, j);
	uint8_t jj2 = jj << 1;
	if( i & 0x80 ) {
		jj2 = 255 - jj2;
	}
	return jj2;
}


int8_t inoise8_raw(uint16_t x, uint16_t y)
{
	// Find the unit cube containing the point
	uint8_t X = x>>8;
	uint8_t Y = y>>8;

	// Hash cube corner coordinates
	uint8_t A = P(X)+Y;
	uint8_t AA = P(A);
	uint8_t AB = P(A+1);
	uint8_t B = P(X+1)+Y;
	uint8_t BA = P(B);
	uint8_t BB = P(B+1);

	// Get the relative position of the point in the cube
	uint8_t u = x;
	uint8_t v = y;

	// Get a signed version of the above for the grad function
	int8_t xx = ((uint8_t)(x)>>1) & 0x7F;
	int8_t yy = ((uint8_t)(y)>>1) & 0x7F;
	uint8_t N = 0x80;

	u = EASE8(u); v = EASE8(v);
	
	int8_t X1 = lerp7by8(grad8(P(AA), xx, yy), grad8(P(BA), xx - N, yy), u);
	int8_t X2 = lerp7by8(grad8(P(AB), xx, yy-N), grad8(P(BB), xx - N, yy - N), u);

	int8_t ans = lerp7by8(X1,X2,v);

	return ans;
	// return scale8((70+(ans)),234)<<1;
}


/// add one byte to another, saturating at 0xFF
/// @param i - first byte to add
/// @param j - second byte to add
/// @returns the sum of i & j, capped at 0xFF
uint8_t qadd8( uint8_t i, uint8_t j)
{
	unsigned int t = i + j;
	if( t > 255) t = 255;
	return t;
}


uint8_t inoise8(uint16_t x, uint16_t y) {
	//return scale8(69+inoise8_raw(x,y),237)<<1;
	int8_t n = inoise8_raw( x, y);  // -64..+64
	n+= 64;                         //   0..128
	uint8_t ans = qadd8( n, n);     //   0..255
	return ans;
}