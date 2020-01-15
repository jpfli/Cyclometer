
#pragma once

#include "Pokitto.h"

#include "State.h"

class StateManager {
    public:
        static void run(State* state);
        static void changeState(State* state);
        
    private:
        explicit StateManager(); // Disabled
        StateManager(const StateManager&); // Disabled
        StateManager& operator =(const StateManager&); // Disabled
        
        static void update();
    
        static State* _state;
        static State* _next_state;
};
