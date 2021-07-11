/*

*/

#include "UTFT_7seg.h"
#include "../UTFT/UTFT.h"

UTFT_7seg::UTFT_7seg(UTFT *ptrUTFT, UTFT_Geometry *ptrUTFT_Geometry)
{
    _UTFT = ptrUTFT;
    _UTFT_Geometry = ptrUTFT_Geometry;

}

void UTFT_7seg::begin(int x, int y, int numDigits, int size, bool firstIsOne)
{
    _numDigits = numDigits;
    _maxValue = _intPower(10,_numDigits) - 1;
    _powStart = _maxValue+1;
    _firstIsOne = firstIsOne;
    if(_firstIsOne) _maxValue-=_intPower(10,_numDigits-1)*8;
    _value = -1;
    _lastDrawnValue = 0;
    for(int i=0; i < _numDigits; i++) _segments[i] = 0x0;
    for(int i=0; i < _numDigits; i++) _lastDrawnSegments[i] = 0x7F;
    _size = size;
    _digitWidth = 16*size;
    _x = x;
    _y = y;
    _redraw = true;

    //set all the segments to OFF and draw the empty display
}

long UTFT_7seg::value(int val)
{
    if(val < 0 || val > _maxValue) val = -1;
    _value = val;
    return _value;
}

long UTFT_7seg::value()
{
    return _value;
}

void UTFT_7seg::_setSegments() {
    long pow = _powStart;
    int j;

    j = 0;
    for (int i = _numDigits; i > 0; i--) {
        pow = pow / 10;
        if (_value >= (pow)) {
            _segments[_numDigits-i] = _segmentBitmap[(int)floor((_value % (pow * 10)) / pow)];
        } else {
            _segments[_numDigits-i] = _segmentBitmap[10];
        }
    }
    if(_value == 0) _segments[_numDigits-1]=_segmentBitmap[0];
}



void UTFT_7seg::draw()
{

    //setup colors
    word color = _UTFT->getColor();
    _r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
    _g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
    _b = (((color & 0x1F) * 527) + 23) >> 6;

    if(_lastDrawnR != _r || _lastDrawnG != _g || _lastDrawnB != _b) _redraw = true;

    if(_value == _lastDrawnValue && !_redraw) {
        //no change...

    } else {
        if(_value == -1) {
            //number not valid...
            for(int i=0; i < _numDigits; i++) _segments[i] = 0;
        } else {
            _setSegments();
        }




        for (int i = 0; i < _numDigits; i++) {
            _drawDigit(i);
        }
        _lastDrawnR = _r;
        _lastDrawnG = _g;
        _lastDrawnB = _b;
        _lastDrawnValue = _value;
        _redraw = false;

    }


}

void UTFT_7seg::draw(int val)
{
    value(val);
    draw();
}

void UTFT_7seg::_drawDigit(int digit)
{
    int digitX = _x + (_digitWidth*digit);
    int barShort = _size * 3;
    int barLong = _size * 7;

    //_UTFT->printNumI(_segments[digit],digitX,_y-20);


    if(_firstIsOne) digitX-= barLong+barShort;

    if(_segments[digit] == _lastDrawnSegments[digit] && !_redraw) {
        //no change
    } else {

        for(int i = 0; i < 7; i++) {
            bool newState = bitRead(_segments[digit],i);
            if(newState != bitRead(_lastDrawnSegments[digit],i) || _redraw) {
                if(newState) {
                    _UTFT->setColor(_r,_g,_b);
                } else {
                    _UTFT->setColor(_r*0.4,_g*0.4,_b*0.4);
                }
                //draw updated segment...
                switch (i) {
                    case 0:
                        // segment A
                        if(_firstIsOne && digit == 0) break;
                        _UTFT->fillRect(digitX+barShort,_y,digitX+barLong+barShort,_y+barShort);
                        _UTFT_Geometry->fillTriangle(digitX+barShort,_y,digitX+barShort,_y+barShort,digitX+barShort/2,_y+barShort/2);
                        _UTFT_Geometry->fillTriangle(digitX+barShort+barLong,_y,digitX+barShort+barLong,_y+barShort,digitX+barLong+barShort+barShort/2,_y+barShort/2);

                        break;
                    case 1:
                        // segment B
                        _UTFT->fillRect(digitX+barLong+barShort,_y+barShort,digitX+barLong+barShort+barShort,_y+barShort+barLong);
                        _UTFT_Geometry->fillTriangle(digitX+barLong+barShort,_y+barShort,digitX+barLong+barShort+barShort,_y+barShort,digitX+barLong+barShort+barShort/2,_y+barShort/2);
                        _UTFT_Geometry->fillTriangle(digitX+barLong+barShort,_y+barShort+barLong,digitX+barLong+barShort+barShort,_y+barShort+barLong,digitX+barLong+barShort+barShort/2,_y+barShort+barLong+barShort/2);
                        break;
                    case 2:
                        // segment C
                        _UTFT->fillRect(digitX+barLong+barShort,_y+barShort+barLong+barShort,digitX+barLong+barShort+barShort,_y+barShort+barLong+barLong+barShort);
                        _UTFT_Geometry->fillTriangle(digitX+barLong+barShort,_y+barShort+barLong+barShort,digitX+barLong+barShort+barShort,_y+barShort+barLong+barShort,digitX+barLong+barShort+barShort/2,_y+barShort+barLong+barShort/2);
                        _UTFT_Geometry->fillTriangle(digitX+barLong+barShort,_y+barShort+barLong+barShort+barLong,digitX+barLong+barShort+barShort,_y+barShort+barLong+barShort+barLong,digitX+barLong+barShort+barShort/2,_y+barShort+barLong+barShort+barLong+barShort/2);

                        break;
                    case 3:
                        // segment D
                        if(_firstIsOne && digit == 0) break;
                        _UTFT->fillRect(digitX+barShort,_y+barLong+barShort+barLong+barShort,digitX+barLong+barShort,_y+barShort+barLong+barShort+barLong+barShort);
                        _UTFT_Geometry->fillTriangle(digitX+barShort,_y+barLong+barShort+barLong+barShort,digitX+barShort,_y+barLong+barShort+barLong+barShort+barShort,digitX+barShort/2,_y+barLong+barShort+barLong+barShort+barShort/2);
                        _UTFT_Geometry->fillTriangle(digitX+barShort+barLong,_y+barLong+barShort+barLong+barShort,digitX+barShort+barLong,_y+barLong+barShort+barLong+barShort+barShort,digitX+barLong+barShort+barShort/2,_y+barLong+barShort+barLong+barShort+barShort/2);

                        break;
                    case 4:
                        // segment E
                        if(_firstIsOne && digit == 0) break;
                        _UTFT->fillRect(digitX,_y+barShort+barLong+barShort,digitX+barShort,_y+barShort+barLong+barLong+barShort);
                        _UTFT_Geometry->fillTriangle(digitX,_y+barShort+barLong+barShort,digitX+barShort,_y+barShort+barLong+barShort,digitX+barShort/2,_y+barShort+barLong+barShort/2);
                        _UTFT_Geometry->fillTriangle(digitX,_y+barShort+barLong+barShort+barLong,digitX+barShort,_y+barShort+barLong+barShort+barLong,digitX+barShort/2,_y+barShort+barLong+barShort+barLong+barShort/2);
                        break;
                    case 5:
                        // segment F
                        if(_firstIsOne && digit == 0) break;
                        _UTFT->fillRect(digitX,_y+barShort,digitX+barShort,_y+barShort+barLong);
                        _UTFT_Geometry->fillTriangle(digitX,_y+barShort,digitX+barShort,_y+barShort,digitX+barShort/2,_y+barShort/2);
                        _UTFT_Geometry->fillTriangle(digitX,_y+barShort+barLong,digitX+barShort,_y+barShort+barLong,digitX+barShort/2,_y+barShort+barLong+barShort/2);
                        break;
                    case 6:
                        // segment G
                        if(_firstIsOne && digit == 0) break;
                        _UTFT->fillRect(digitX+barShort,_y+barShort+barLong,digitX+barLong+barShort,_y+barShort+barShort+barLong);
                        _UTFT_Geometry->fillTriangle(digitX+barShort,_y+barShort+barLong,digitX+barShort,_y+barShort+barLong+barShort,digitX+barShort/2,_y+barShort+barLong+barShort/2);
                        _UTFT_Geometry->fillTriangle(digitX+barShort+barLong,_y+barShort+barLong,digitX+barShort+barLong,_y+barShort+barLong+barShort,digitX+barLong+barShort+barShort/2,_y+barShort+barLong+barShort/2);

                        break;
                }
            }
        }
        _lastDrawnSegments[digit] = _segments[digit];
        _UTFT->setColor(_r,_g,_b);
    }

}

long UTFT_7seg::_intPower(uint8_t base, uint8_t exp)
{
    long result = 1;

    while (exp--) {
        result *= base;
    }

    return result;
}