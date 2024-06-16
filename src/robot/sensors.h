#pragma once
#include <memory>

#include "utils.h"
#include "tile.h"

class Robot;

class Sensor {
    protected:
        std::shared_ptr<TileLayout> layout;
        Robot& robot;
    public:
        Sensor(std::shared_ptr<TileLayout> layout, Robot& r) : layout(layout), robot(r) {}
        Sensor(Robot& r) : robot(r) {}
};


class DirtSensor : public Sensor {
public:
    DirtSensor(std::shared_ptr<TileLayout> layout, Robot& r) : Sensor(layout, r){
        // Initialize the sensor
    }

    bool isDirty(); // Check if the current tile is dirty.

    int DirtLevel(); // Return the dirt level.
};

class WallSensor : public Sensor {
    
public:
    WallSensor(std::shared_ptr<TileLayout> layout, Robot& r) : Sensor(layout, r) {
        // Initialize the sensor
    }

    bool isWall(Direction direction);// Check if the sensor is detecting a wall/
};

class BatterySensor: public Sensor{
public:
    BatterySensor(Robot& r) : Sensor(r){
        // Initialize the sensor
    }

    int BatteryLevel(); // Return the battery level.
};