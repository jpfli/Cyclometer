
#pragma once

#include <cstdint>
#include "State.h"
#include "Cyclometer.h"
#include "CircularBar.h"

class CyclometerDisplay : public State {
    public:
        explicit CyclometerDisplay();
        ~CyclometerDisplay() {}
    
    public: // State
        void initialize();
        void update();
        void terminate() {}
    
    private:
        CyclometerDisplay(const CyclometerDisplay&); // Disabled
        CyclometerDisplay& operator =(const CyclometerDisplay&); // Disabled
        
        static constexpr uint16_t PALETTE_RGB565[] = {
            0x0000, 0xFFFF, 0xA534, 0x87F9, 0xA286, 0x5AA9, 0xBE18, 0xFF7C, 
            0xF809, 0xFD00, 0xFF44, 0x0706, 0x255F, 0x7BB2, 0xFBB4, 0xFE54
        };
    
        void printRideSpeed();
        void printRideTime();
        void printRideDistance();
        
        Cyclometer _cyclometer;
        
        CircularBar _ride_speed_bar;
        CircularBar _avg_speed_bar;
};
