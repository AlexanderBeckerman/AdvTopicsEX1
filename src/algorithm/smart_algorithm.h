#pragma once
#include "abstract_algorithm.h"
#include "config.h"
#include "expanding_map.h"
#include "relative_point.h"
#include "sensors.h"
#include "utils.h"
#include <optional>
#include <stack>
#include <unordered_set>

class SmartAlgorithm : public AbstractAlgorithm {
    // Configuration.
    std::shared_ptr<DirtSensor> dirt_sensor;
    std::shared_ptr<WallsSensor> wall_sensor;
    std::shared_ptr<BatteryMeter> battery_sensor;
    size_t max_steps;
    RelativePoint robot_location;

    // BFS state
    std::unordered_set<RelativePoint, RelativePointKeyHash> visited;
    std::stack<Direction> direction_stack;

    std::optional<std::stack<Direction>> return_path;

    bool isValidMove(const Direction &direction) const {
        if (wall_sensor->isWall(direction))
            return false;

        // Not facing a wall.
        auto new_point = robot_location + direction;
        return visited.find(new_point) == visited.end();
    }

    void startReturn();

  public:
    SmartAlgorithm() = default;
    void setMaxSteps(std::size_t maxSteps) override {
        this->max_steps = maxSteps;
    }
    void setWallsSensor(const WallsSensor &ws) override {
        this->wall_sensor = std::make_shared<ConcreteWallSensor>(
            dynamic_cast<const ConcreteWallSensor &>(ws));
    }
    void setDirtSensor(const DirtSensor &ds) override {
        this->dirt_sensor = std::make_shared<ConcreteDirtSensor>(
            dynamic_cast<const ConcreteDirtSensor &>(ds));
    }
    void setBatteryMeter(const BatteryMeter &bm) override {
        this->battery_sensor = std::make_shared<ConcreteBatteryMeter>(
            dynamic_cast<const ConcreteBatteryMeter &>(bm));
    }

    Step nextStep() override;
};