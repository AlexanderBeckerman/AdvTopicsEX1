#pragma once

#include "../utils/layout_point.h"
#include "../utils/tile.h"
#include "../utils/utils.h"

class Robot;
typedef std::vector<std::vector<Tile>> TileLayout;

class WallsSensor {
  public:
    virtual ~WallsSensor() {}
    virtual bool isWall(Direction d) const = 0;
};

class DirtSensor {
  public:
    virtual ~DirtSensor() {}
    virtual int dirtLevel() const = 0;
};

class BatteryMeter {
  public:
    virtual ~BatteryMeter() {}
    virtual std::size_t getBatteryState() const = 0;
};

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

class ConcreteDirtSensor : public LayoutReader, public DirtSensor {
  public:
    using LayoutReader::LayoutReader;
    bool isDirty() const;
    int dirtLevel() const override;
    Tile &getDirtyTile() {
        Tile &tile = (*layout)[location.row][location.col];
        return tile;
    }
    friend class AlgorithmTest;
};

class ConcreteWallSensor : public LayoutReader, public WallsSensor {
  public:
    using LayoutReader::LayoutReader;
    bool isWall(Direction d) const override;
};

class ConcreteBatteryMeter : public BatteryMeter {
    size_t capacity;
    size_t charge;
    size_t steps_at_charging = 0;

  public:
    ConcreteBatteryMeter(int capacity, int charge = 0)
        : capacity(capacity), charge(charge) {}

    size_t getBatteryState() const override {
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