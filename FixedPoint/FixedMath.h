#pragma once

#include "Fixed.h"

namespace FixedPoint {
    constexpr const Fixed Pi = Fixed(3.1415926536);
    
    Fixed sin(Fixed radians);
    
    Fixed cos(Fixed radians);
    
    Fixed tan(Fixed radians);
    
    Fixed atan2(Fixed y, Fixed x);
} // namespace FixedPoint
