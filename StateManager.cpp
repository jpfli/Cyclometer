
#include <Pokitto.h>
#include "StateManager.h"

State* StateManager::_state = nullptr;
State* StateManager::_next_state = nullptr;

void StateManager::run(State* state) {
    changeState(state);
    
    while(Pokitto::Core::isRunning()) {
        if(Pokitto::Core::update()) {
            update();
        }
    }
}

void StateManager::changeState(State* state) {
    _next_state = state;
}

void StateManager::update() {
    if(_next_state != nullptr) {
        if(_state != nullptr) {
            _state->terminate();
        }
        _next_state->initialize();
        _state = _next_state;
        _next_state = nullptr;
    }
    _state->update();
}
