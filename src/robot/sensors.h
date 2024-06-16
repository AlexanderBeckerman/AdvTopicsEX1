#pragma once
#include <memory>

#include "utils.h"
#include "tile.h"

class DirtSensor {
    std::shared_ptr<TileLayout> layout;
public:
    DirtSensor(std::shared_ptr<TileLayout> layout) : layout(layout){
        // Initialize the sensor
    }

    bool isDirty() {
        // Check if the sensor is dirty
        return false;
    }

    int DirtLevel() {
        // Return the dirt level
        return 0;
    }
};

class WallSensor {
public:
    WallSensor() {
        // Initialize the sensor
    }

    bool isWall(Direction direction) {
        // Check if the sensor is detecting a wall
        return false;
    }
};

class BatterySensor {
public:
    BatterySensor() {
        // Initialize the sensor
    }

    int BatteryLevel() {
        // Return the battery level
        return 0;
    }
};