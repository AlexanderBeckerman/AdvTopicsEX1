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
    virtual ~ConcreteDirtSensor() {}
    bool isDirty() const { return this->getCurrentTile().getDirtLevel() > 0; }
    int dirtLevel() const override {
        return this->getCurrentTile().getDirtLevel();
    }
    Tile &getDirtyTile() {
        Tile &tile = (*layout)[location.row][location.col];
        return tile;
    }
    friend class AlgorithmTest;
};

class ConcreteWallSensor : public LayoutReader, public WallsSensor {
  public:
    using LayoutReader::LayoutReader;
    bool isWall(const Direction direction) const override {
        size_t row = location.row;
        size_t col = location.col;
        switch (direction) {
        case Direction::North:
            if (row == 0) {
                return true;
            }
            break;
        case Direction::South:
            if (row == layout->size() - 1) {
                return true;
            }
            break;
        case Direction::West:
            if (col == 0) {
                return true;
            }
            break;
        case Direction::East:
            if (col == (*layout)[row].size() - 1) {
                return true;
            }
            break;
        default:
            return false;
        };
        auto layout_point = location + direction;
        return (*layout)[layout_point.row][layout_point.col].getType() ==
               TileType::WALL;
    }
    virtual ~ConcreteWallSensor() {}
};

class ConcreteBatteryMeter : public BatteryMeter {
    size_t capacity;
    size_t charge;
    size_t steps_at_charging = 0;

  public:
    virtual ~ConcreteBatteryMeter() {}
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