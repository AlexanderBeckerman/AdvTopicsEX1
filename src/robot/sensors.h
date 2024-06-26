#pragma once

#include "utils.h"
#include "tile.h"
#include "config.h"

class Robot;

class Sensor
{
protected:
    std::shared_ptr<TileLayout> layout;
    
    Robot &robot;

public:
    Sensor(std::shared_ptr<TileLayout> &layout, Robot &r) : layout(layout), robot(r) {}
    Sensor(Robot &r) : robot(r) {}
};

class DirtSensor : public Sensor
{
public:
    DirtSensor(std::shared_ptr<TileLayout> layout, Robot &r) : Sensor(layout, r)
    {
        // Initialize the sensor
    }

    bool isDirty(); // Check if the current tile is dirty.

    int DirtLevel(); // Return the dirt level.
    Tile &getCurrentTile();
};

class WallSensor : public Sensor
{

public:
    WallSensor(std::shared_ptr<TileLayout> layout, Robot &r) : Sensor(layout, r)
    {
        // Initialize the sensor
    }

    bool isWall(Direction direction); // Check if the sensor is detecting a wall/
    Tile getWallTile(Direction d) const;
};

class BatterySensor
{
    int capacity;
    int charge;
    size_t steps_at_charging = 0;

public:
    BatterySensor(int capacity, int charge = 0) : capacity(capacity), charge(charge) {}

    int batteryLevel(){
        // Update battery level.
        if (steps_at_charging != 0)
            return std::min(capacity, (static_cast<int>(charge + static_cast<float>(steps_at_charging) * (static_cast<float>(capacity) / 20))));
        
        return charge;
    }

    void chargeBattery(){
        // Charge the battery.
        steps_at_charging++;
    }

    void stopCharging(){
        // Stop charging the battery.
        charge = std::min(capacity, (static_cast<int>(charge + static_cast<float>(steps_at_charging) * (static_cast<float>(capacity) / 20))));
        steps_at_charging = 0;
    }

    int getCapacity() const {
        return capacity;
    }

    void decreaseCharge(){
        // Decrease the battery level.
        charge--;
    }
};