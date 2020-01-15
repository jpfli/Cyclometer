
#include <cstdint>
#include <mbed.h>
#include <Pokitto.h>
#include "System.h"
#include "Cyclometer.h"

uint32_t Cyclometer::_time_of_last_interrupt(0);
volatile uint32_t Cyclometer::_time_of_last_edge(0);

Cyclometer::Cyclometer() : _time_of_last_pulse(0), _pin_ext0(EXT0), _pin_buttonB(POK_BTN_B_PIN), _is_buttonB_pressed(false),_rev_time(0), _ride_time(0), _num_revs(0), _speed(0) {
    _pin_ext0.mode(PullDown);
    
    // Attach interrupt hadlers for rising and falling edge
    _pin_ext0.rise(&handleRise);
    _pin_ext0.fall(&handleFall);
}

Cyclometer::~Cyclometer() {
    _pin_ext0.rise(nullptr);
    _pin_ext0.fall(nullptr);
}

uint32_t Cyclometer::rideSpeed() const {
    return _speed;
}

uint32_t Cyclometer::rideTimeMillis() const {
    return _ride_time;
}

uint32_t Cyclometer::rideDistance() const {
    return static_cast<uint64_t>(CIRCUMFERENCE) * _num_revs / 1000;
}

uint32_t Cyclometer::averageSpeed() const {
    return static_cast<uint64_t>(SECS_IN_HOUR * CIRCUMFERENCE) * _num_revs / _ride_time;
}

void Cyclometer::update() {
    if(_pin_buttonB.read() == 0) {
        _is_buttonB_pressed = false;
    }
    else if(!_is_buttonB_pressed) {
        _is_buttonB_pressed = true;
        handleRise();
    }
    
    // Check if a new pulse was detected and calculate revolution time
    uint32_t time_of_edge = _time_of_last_edge;  // snap a copy of volatile variable
    if(0 < time_of_edge - _time_of_last_pulse) {
        _rev_time = time_of_edge - _time_of_last_pulse;
        _time_of_last_pulse = time_of_edge;
        if(_rev_time <= TIMEWIN) {
            _ride_time += _rev_time;
            ++_num_revs;
        }
    }
    
    // Use the greater of idle_time and _rev_time to calculate speed
    uint32_t idle_time = System::currentTimeMillis() - _time_of_last_pulse;
    uint32_t delta = (idle_time > _rev_time) ? idle_time : _rev_time;
    _speed = 0;
    if(delta <= TIMEWIN) {
        _speed = (SECS_IN_HOUR * CIRCUMFERENCE) / delta; // Speed in meters/h
    }
}

void Cyclometer::handleRise() {
    uint32_t now = System::currentTimeMillis();
    if(now - _time_of_last_interrupt > DEBOUNCE_TIME) {
        // Only save this edge as the latest if no other edge has been detected
        // within _DEBOUNCE_TIME milliseconds
        _time_of_last_edge = now;
    }
    _time_of_last_interrupt = now;
}

void Cyclometer::handleFall() {
    _time_of_last_interrupt = System::currentTimeMillis();
}
