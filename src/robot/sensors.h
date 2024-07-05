#pragma once

#include "config.h"
#include "tile.h"
#include "utils.h"

class Robot;

class LayoutReader {
  protected:
    std::shared_ptr<TileLayout> layout;
    LayoutPoint location;

  public:
    explicit LayoutReader(std::shared_ptr<TileLayout> layout,
                          const LayoutPoint &location)
        : layout(layout), location(location) {}

    const Tile &getCurrentTile() const {
        return (*layout)[location.row][location.col];
    }

    void step(Direction direction) { location = location + direction; }

    friend Robot;
};

class DirtSensor : public LayoutReader {
  public:
    using LayoutReader::LayoutReader;
    bool isDirty() const;
    int DirtLevel() const;
    Tile &getDirtyTile() {
        Tile &tile = (*layout)[location.row][location.col];
        return tile;
    }
};

class WallSensor : public LayoutReader {
  public:
    using LayoutReader::LayoutReader;
    bool isWall(const Direction direction) const;
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