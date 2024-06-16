#pragma once
#include "config.h"
#include "expanding_map.h"
#include "sensors.h"

class Algorithm {
    // State.
    ExpandingMap map;
    Location curr_pos;

    public:

    Algorithm() {
        this->map = ExpandingMap();
        this->curr_pos = Location(0, 0);
    }
    
    Direction nextMove() {
        // Implementation of nextMove function
        return Direction::UP;
    }
};