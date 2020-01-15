#pragma once

#include <stdint.h>
#include "FixedPoint/Fixed.h"

class CircularBar
{
    public:
        CircularBar(int16_t x, int16_t y, int16_t rmajor, int16_t rminor, uint8_t color);
        ~CircularBar();
        
        int16_t getOriginX() { return _x; }
        void setOriginX(int16_t val) { _x = val; }
        int16_t getOriginY() { return _y; }
        void setOriginY(int16_t val) { _y = val; }
        
        int16_t getMajorRadius() { return _rmaj; }
        void setMajorRadius(int16_t val) { _rmaj = val; }
        int16_t getMinorRadius() { return _rmin; }
        void setMinorRadius(int16_t val) { _rmin = val; }
        
        void setColor(uint8_t val) { _color = val; }
        
        void setStartAngle(int16_t degrees);
        void setEndAngle(int16_t degrees);
        
        void draw();
    
    private:
        CircularBar(const CircularBar&); // Disabled
        CircularBar& operator =(const CircularBar&); // Disabled
        
        void drawScanline(int16_t x0, int16_t x1, int16_t y);
        void drawVertline(int16_t x0, int16_t x1, int16_t y);
        bool isInsideSector(int16_t x, int16_t y);
        
        int16_t _x;
        int16_t _y;
        int16_t _rmaj;
        int16_t _rmin;
        uint8_t _color;
        FixedPoint::Fixed _vstart[2];
        FixedPoint::Fixed _vend[2];
        bool _is_angle_reflex;
};
