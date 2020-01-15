
#include <Pokitto.h>
#include "CircularBar.h"
#include "FixedPoint/Fixed.h"
#include "FixedPoint/FixedMath.h"

CircularBar::CircularBar(int16_t x, int16_t y, int16_t rmajor, int16_t rminor, uint8_t color):
    _x(x), _y(y), _rmaj(rmajor), _rmin(rminor), _color(color), 
    _vstart{FixedPoint::Fixed(1), FixedPoint::Fixed(0)}, _vend{FixedPoint::Fixed(1), FixedPoint::Fixed(0)}, 
    _is_angle_reflex(false)
{
}

CircularBar::~CircularBar()
{
}

void CircularBar::setStartAngle(int16_t degrees)
{
    FixedPoint::Fixed angle = degrees * FixedPoint::Pi / 180;
    _vstart[0] = cos(angle);
    _vstart[1] = sin(angle);
}

void CircularBar::setEndAngle(int16_t degrees)
{
    FixedPoint::Fixed angle = degrees * FixedPoint::Pi / 180;
    _vend[0] = cos(angle);
    _vend[1] = sin(angle);
}

void CircularBar::draw()
{
    // Combines two circle algorithms to draw a circular bar. Calculates x0
    // and x1 for for each horizontal line of the first 45 deg sector of the
    // circular bar. For other sectors the coordinates are flipped. In the
    // line drawing functions a per pixel check is used to only draw pixels
    // between start and end angle.
    
    int16_t x0 = _rmin;
    int16_t x1 = _rmaj;
    int16_t y = 0;
    int16_t x0_change = 1 - (_rmin << 1);
    int16_t x1_change = 1 - (_rmaj << 1);
    int16_t y_change = 1;
    int16_t r1_err = 0;
    int16_t r2_err = 0;

    _is_angle_reflex = (_vstart[1] * _vend[0]) > (_vstart[0] * _vend[1]);

    while(x0 >= y) {
        drawScanline(x0, x1, y);
        drawScanline(-x1, -x0, y);
        drawScanline(x0, x1, -y);
        drawScanline(-x1, -x0, -y);
        drawVertline(y, x0, x1);
        drawVertline(y, -x1, -x0);
        drawVertline(-y, x0, x1);
        drawVertline(-y, -x1, -x0);
        ++y;

        r1_err += y_change;
        r2_err += y_change;
        y_change += 2;
        if((r1_err << 1) + x0_change > 0) {
            --x0;
            r1_err += x0_change;
            x0_change += 2;
        }
        if((r2_err << 1) + x1_change > 0) {
            --x1;
            r2_err += x1_change;
            x1_change += 2;
        }
    }
    while(x1 >= y) {
        drawScanline(y, x1, y);
        drawScanline(-x1, -y, y);
        drawScanline(y, x1, -y);
        drawScanline(-x1, -y, -y);
        drawVertline(y, y + 1, x1);
        drawVertline(y, -x1, -y - 1);
        drawVertline(-y, y + 1, x1);
        drawVertline(-y, -x1, -y - 1);
        ++y;

        r2_err += y_change;
        y_change += 2;
        if((r2_err << 1) + x1_change > 0) {
            --x1;
            r2_err += x1_change;
            x1_change += 2;
        }
    }
}

void CircularBar::drawScanline(int16_t x0, int16_t x1, int16_t y)
{
    while(x0 <= x1) {
        if(isInsideSector(x0, y)) {
          Pokitto::Display::drawPixelRaw(_x+x0, _y+y, _color);
        }
        ++x0;
    }
}

void CircularBar::drawVertline(int16_t x, int16_t y0, int16_t y1)
{
    while(y0 <= y1) {
        if(isInsideSector(x, y0)) {
          Pokitto::Display::drawPixelRaw(_x + x, _y + y0, _color);
        }
        ++y0;
    }
}

bool CircularBar::isInsideSector(int16_t x, int16_t y)
{
    // // Uses 2D vector cross product to check, which side of start and end vectors the pixel is located.
    bool eq1 = _vstart[1] * x < _vstart[0] * y;
    bool eq2 = _vend[1] * x > _vend[0] * y;
    return _is_angle_reflex ? (eq1 || eq2) : (eq1 && eq2);
}