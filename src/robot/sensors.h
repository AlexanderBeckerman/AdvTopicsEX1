#pragma once

#include "config.h"
#include "tile.h"
#include "utils.h"

class Robot;

class DirtSensor {
    std::shared_ptr<TileLayout> layout;
    Robot &robot;

  public:
    DirtSensor(std::shared_ptr<TileLayout> layout, Robot &r)
        : layout(layout), robot(r) {}

    bool isDirty() const;

    int DirtLevel() const;
    Tile &getCurrentTile();
};

class WallSensor {
    std::shared_ptr<TileLayout> layout;
    Robot &robot;

  public:
    WallSensor(std::shared_ptr<TileLayout> layout, Robot &r)
        : layout(layout), robot(r) {}

    bool isWall(const Direction direction) const;
    Tile getWallTile(const Direction d) const;
};

class BatterySensor {
    size_t capacity;
    size_t charge;
    size_t steps_at_charging = 0;

  public:
    BatterySensor(int capacity, int charge = 0)
        : capacity(capacity), charge(charge) {}

    size_t batteryLevel() const {
        // Update battery level.
        if (steps_at_charging != 0)
            return std::min(
                capacity,
                (static_cast<size_t>(charge +
                                     (steps_at_charging) *
                                         (static_cast<float>(capacity) / 20))));

        return charge;
    }

    void chargeBattery() {
        // Charge the battery.
        steps_at_charging++;
    }

    void stopCharging() {
        // Stop charging the battery.
        charge = std::min(
            capacity, (static_cast<size_t>(
                          charge + static_cast<float>(steps_at_charging) *
                                       (static_cast<float>(capacity) / 20))));
        steps_at_charging = 0;
    }

    size_t getCapacity() const { return capacity; }

    void decreaseCharge() {
        // Decrease the battery level.
        charge--;
    }
};