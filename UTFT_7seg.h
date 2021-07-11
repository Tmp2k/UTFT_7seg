/*

*/

#ifndef UTFT_7seg_h
#define UTFT_7seg_h

#define UTFT_7SEG_VERSION	101

#define UTFT_7SEG_BITMAP
#define UTFT_7SEG_MAX_DIGITS 10

#include "../UTFT/UTFT.h"
#include "../UTFT_Geometry/UTFT_Geometry.h"

#if ((!defined(UTFT_VERSION)) || (UTFT_VERSION<241))
	#error : You will need UTFT v2.41 or higher to use this add-on library...
#endif
#if ((!defined(UTFT_GEOMETRY_VERSION)) || (UTFT_GEOMETRY_VERSION<102))
    #error : You will need UTFT Geometry v2.01 or higher to use this add-on library...
#endif


class UTFT_7seg
{
	public:
		UTFT_7seg(UTFT *ptrUTFT, UTFT_Geometry *ptrUTFT_Geometry);

		void    begin(int x, int y, int numDigits = 1, int size = 10, bool firstIsOne = false);
        long	value(int val);
		long    value();
        void    draw();
        void    draw(int val);


	protected:
		UTFT    *_UTFT;
        UTFT_Geometry *_UTFT_Geometry;
		bool    _firstIsOne;
        int     _numDigits;
        int     _size;
        int     _digitWidth;
        int     _x;
        int     _y;
        int     _r;
        int     _g;
        int     _b;
        int     _lastDrawnR;
        int     _lastDrawnG;
        int     _lastDrawnB;
        long    _value;
        long    _lastDrawnValue;
        bool    _redraw;
        byte    _segments[UTFT_7SEG_MAX_DIGITS] = {0};
        byte    _lastDrawnSegments[UTFT_7SEG_MAX_DIGITS] = {0};
        long    _maxValue;
        long    _powStart;

        byte    _segmentBitmap[11] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00};

        long    _intPower(uint8_t base, uint8_t exp);
        void    _drawDigit(int digit);
        void    _setSegments();
};

#endif
