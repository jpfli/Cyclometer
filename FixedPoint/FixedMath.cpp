
#include "FixedMath.h"
#include "Fixed.h"

using namespace FixedPoint;

Fixed FixedPoint::sin(Fixed radians) {
  // Limit angle to -90..270
  while( -Pi/2 > radians ) {
    radians += 2*Pi;
  }
  while( 3*Pi/2 <= radians ) {
    radians -= 2*Pi;
  }
  // Mirror angles larger than 90 degrees
  if( Pi/2 < radians ) {
    radians = Pi-radians;
  }
  
  // For angles close to -90 and 90 degrees use cosine approximation
  constexpr const Fixed LIMIT = 52.65 * Pi / 180;
  if( LIMIT <= radians ) {
    radians -= Pi / 2;
    Fixed angle_sq = radians * radians;
    return 1 - angle_sq * (Fixed(0.5) - angle_sq * Fixed(0.041672));
  }
  else if( -LIMIT >= radians ) {
    radians += Pi / 2;
    Fixed angle_sq = radians*radians;
    return angle_sq * (Fixed(0.5) - angle_sq * Fixed(0.041672)) - 1;
  }
  
  // For all other angles use sine approximation
  Fixed angle_sq = radians*radians;
  return radians * (1 - angle_sq * (Fixed(0.1666718) - angle_sq * Fixed(0.0083313)));
}

Fixed FixedPoint::cos(Fixed radians) {
  return sin(radians + Pi / 2);
}
    
Fixed FixedPoint::tan(Fixed radians) {
  return sin(radians) / cos(radians);
}
    
Fixed FixedPoint::atan2(Fixed y, Fixed x) {
  Fixed abs_y = (y < Fixed(0)) ? -y : y;
  
  Fixed angle = Fixed(0);
  if(x >= Fixed(0)) {
    Fixed r = (x - abs_y) / (x + abs_y);
    angle = Fixed::fromInternal(0x00003240) * r * r * r - Fixed::fromInternal(0x0000FB50) * r + Pi / 4;
  }
  else {
    Fixed r = (x + abs_y) / (abs_y - x);
    angle = Fixed::fromInternal(0x00003240) * r * r * r - Fixed::fromInternal(0x0000FB50) * r + 3 * Pi / 4;
  }
  
  if(y < Fixed(0)) {
    return -angle;
  }
  return angle;
}
