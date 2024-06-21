#pragma once
#include "config.h"
#include "expanding_map.h"
#include "sensors.h"
#include <stack>

class Algorithm
{
    // State.
    ExpandingMap map;
    Location curr_pos;
    DirtSensor &dirt_sensor;
    WallSensor &wall_sensor;
    BatterySensor &battery_sensor;
    std::stack<Direction> path;
    int steps_since_charge = 0;

    // Helper functions.
    bool shouldMove(Direction d);
    Direction returnToChargingStation();
    bool notEnoughBattery();
    void setLocation(Direction d);
    void updateMap(Direction d);

public:
    Algorithm(DirtSensor &ds, WallSensor &ws, BatterySensor &bs) : dirt_sensor(ds), wall_sensor(ws), battery_sensor(bs)
    {
        this->map = ExpandingMap();
        this->curr_pos = Location(0, 0);
        this->map.addTile(this->curr_pos, dirt_sensor.getCurrentTile()); // Add the charging station tile.
    }

    Direction nextMove();
};