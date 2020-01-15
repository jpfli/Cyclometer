
#include <Pokitto.h>
#include "StateManager.h"
#include "State.h"
#include "CyclometerDisplay.h"

int main() {
    Pokitto::Core::begin();
    Pokitto::Display::persistence = true;
    Pokitto::Display::invisiblecolor = 0;
    
    CyclometerDisplay disp;
    StateManager::run(&disp);
    
    return 0;
}
