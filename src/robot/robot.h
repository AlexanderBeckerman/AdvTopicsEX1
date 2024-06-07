#pragma once
#include "map.h"
#include "utils.h"

class robot {
    Map map;
    Position position;

public:
    robot(Map map) : map(map) {}

    void move(Direction direction);
    Position getPosition(){
        return position;
    }
};

