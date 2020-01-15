
#pragma once

class State {
    public:
        virtual void initialize() = 0;
        virtual void update() = 0;
        virtual void terminate() = 0;
};
