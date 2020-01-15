
#include <cstdint>
#include "CyclometerDisplay.h"
#include "images/BackgroundImage.h"

extern const unsigned char font3x9[];
extern const unsigned char font10x16[];

constexpr uint16_t CyclometerDisplay::PALETTE_RGB565[];

CyclometerDisplay::CyclometerDisplay() : _cyclometer(), _ride_speed_bar(55, 44, 43, 38, 3), _avg_speed_bar(55, 44, 43, 41, 2) {
    
}

void CyclometerDisplay::initialize() {
    for(uint8_t idx=0; idx<16; ++idx) {
        Pokitto::Display::palette[idx] = PALETTE_RGB565[idx];
    }
}

void CyclometerDisplay::update() {
    _cyclometer.update();
    
    // Draw the background image with speedometer scale
    uint8_t tmp = Pokitto::Display::invisiblecolor;
    Pokitto::Display::invisiblecolor = -1;
    Pokitto::Display::drawBitmap(0, 0, BackgroundImage);
    Pokitto::Display::invisiblecolor = tmp;
    
    printRideSpeed();
    printRideTime();
    printRideDistance();
    
    // Draw average speed as a circular bar
    uint32_t avg_speed = _cyclometer.averageSpeed(); // speed in meters/h
    uint32_t avg_angle = (avg_speed < 50000) ? 150 + (avg_speed * 300 / 50000) : 150 + 300;
    _avg_speed_bar.setStartAngle(150);
    _avg_speed_bar.setEndAngle(avg_angle);
    _avg_speed_bar.draw();
    
    // Draw current ride speed on top of the average speed bar
    uint32_t ride_speed = _cyclometer.rideSpeed(); // speed in meters/h
    uint32_t ride_angle = (ride_speed < 50000) ? 150 + (ride_speed * 300 / 50000) : 150 + 300;
    _ride_speed_bar.setStartAngle(150);
    _ride_speed_bar.setEndAngle(ride_angle);
    _ride_speed_bar.draw();
}

void CyclometerDisplay::printRideSpeed() {
    uint32_t integer_kmh = _cyclometer.rideSpeed() / 1000;
    uint8_t fraction_kmh = (_cyclometer.rideSpeed() / 100) % 10;
    
    uint8_t x = (integer_kmh < 10) ? 47 : (integer_kmh < 100) ? 37 : 27;
    Pokitto::Display::set_cursor(x, 37);
    Pokitto::Display::setFont(font10x16);
    Pokitto::Display::adjustCharStep = 0;
    Pokitto::Display::setColor(3);
    
    Pokitto::Display::printf("%u #", integer_kmh);
    
    Pokitto::Display::set_cursor(56, 43);
    Pokitto::Display::setFont(font3x9);
    Pokitto::Display::adjustCharStep = 1;
    Pokitto::Display::setColor(3);
    
    Pokitto::Display::printf(".%u", fraction_kmh);
}

void CyclometerDisplay::printRideTime() {
    uint32_t rideTime = _cyclometer.rideTimeMillis(); // ride time in milliseconds
    uint8_t hours = (rideTime / (3600 * 1000)) % 100;
    uint8_t mins = (rideTime / (60 * 1000)) % 60;
    uint8_t secs = (rideTime / 1000) % 60;
    
    uint8_t x = (hours < 10) ? 44 : 40;
    Pokitto::Display::set_cursor(x, 23);
    Pokitto::Display::setFont(font3x9);
    Pokitto::Display::setColor(1);
    
    Pokitto::Display::printf("%u:%02u:%02u", hours, mins, secs);
}

void CyclometerDisplay::printRideDistance() {
    uint32_t rideDist = _cyclometer.rideDistance(); // distance in meters
    uint32_t integer_km = rideDist / 1000;
    uint8_t fraction_km = (rideDist / 100) % 10;
    
    uint8_t x = 48;
    
    // Change printing position based on how many digits there is in the value
    uint32_t ref = 10;
    while(ref <= integer_km) {
        x -= 4;
        ref *= 10;
    }
    
    Pokitto::Display::set_cursor(x, 57);
    Pokitto::Display::setFont(font3x9);
    Pokitto::Display::setColor(1);
    
    Pokitto::Display::printf("%u.%u km", integer_km, fraction_km);
}
