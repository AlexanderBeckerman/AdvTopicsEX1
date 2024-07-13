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
    const DirtSensor* dirt_sensor;
    const WallsSensor* wall_sensor;
    const BatteryMeter* battery_sensor;
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
    SmartAlgorithm() {
        max_steps = 0;
        robot_location = {0, 0};
        visited.insert(robot_location);
    }
    void setMaxSteps(std::size_t maxSteps) override {
        this->max_steps = maxSteps;
    }
    void setWallsSensor(const WallsSensor &ws) override {
        this->wall_sensor = &ws;
    }
    void setDirtSensor(const DirtSensor &ds) override {
        this->dirt_sensor = &ds;
    }
    void setBatteryMeter(const BatteryMeter &bm) override {
        this->battery_sensor = &bm;
    }

    Step nextStep() override;
};