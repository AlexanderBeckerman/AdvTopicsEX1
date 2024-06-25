#pragma once
#include "config.h"
#include "expanding_map.h"
#include <stack>
#include "sensors.h"
#include <vector>

class Algorithm
{
    // State.
    ExpandingMap map;
    DirtSensor &dirt_sensor;
    WallSensor &wall_sensor;
    BatterySensor &battery_sensor;
    Location curr_pos;
    std::stack<Direction> path;
    int steps_since_charge = 0;

    // Helper functions.
    bool shouldMove(Direction d);
    Direction returnToChargingStation();
    bool notEnoughBattery();
    void setLocation(Direction d);
    void updateMap(Direction d = Direction::STAY); // When we won't pass argument then it means we are updating with current tile, else updating a wall.
    std::vector<Direction> getPossibleDirections();


public:
    Algorithm(DirtSensor &ds, WallSensor &ws, BatterySensor &bs) : dirt_sensor(ds), wall_sensor(ws), battery_sensor(bs), map(ExpandingMap()), curr_pos(Location(0, 0))
    {
        Tile t(this->curr_pos, TileType::CHARGING_STATION);
        this->map.addTile(this->curr_pos, t); // Add the charging station tile.
    }

    Direction nextMove();
};