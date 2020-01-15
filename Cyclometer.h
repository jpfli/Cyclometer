
#pragma once

#include <cstdint>
#include <Pokitto.h>

class Cyclometer {
    public:
        explicit Cyclometer();
        ~Cyclometer();
        
        // Returns ride speed in meters/hour
        uint32_t rideSpeed() const;
        
        // Returns ride time in milliseconds
        uint32_t rideTimeMillis() const;
        
        // Returns distance in meters
        uint32_t rideDistance() const;
        
        // Returns average speed in meters/hour
        uint32_t averageSpeed() const;
        
        void update();
    
    private:
        Cyclometer(const Cyclometer&); // Disabled
        Cyclometer& operator =(const Cyclometer&); // Disabled
        
        static void handleRise();
        static void handleFall();
        
    private:
        static constexpr uint32_t CIRCUMFERENCE = 2120; // millimeters
        
        static constexpr uint32_t DEBOUNCE_TIME = 30; // milliseconds
        static constexpr uint32_t TIMEWIN = 3000; // milliseconds
        static constexpr uint32_t SECS_IN_HOUR = 3600;
        
        static uint32_t _time_of_last_interrupt;
        static volatile uint32_t _time_of_last_edge;
        
        uint32_t _time_of_last_pulse;
        
        InterruptIn _pin_ext0;
        DigitalIn _pin_buttonB;
        
        bool _is_buttonB_pressed;
        
        uint32_t _rev_time;
        uint32_t _ride_time;
        uint32_t _num_revs;
        uint32_t _speed;
};
