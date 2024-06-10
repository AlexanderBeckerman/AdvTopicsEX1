#pragma once
#include "utils.h"
#include "map.h"

class Map;

class Robot {
    Map map;
    Position position;
    size_t max_steps; 
    size_t max_battery_steps; 
    Position charging_station; 

public:
    Robot(Map map) : map(map) {}
    Robot(std::string path) : map(path, *this) {}

    void print();
    void setMaxSteps(size_t max_steps);
    void setMaxBatterySteps(size_t max_battery_steps);
    void move(Direction direction);
    void clean(Position p);
    Map& getMap();
    Position getPosition(){
        return position;
    }
};

