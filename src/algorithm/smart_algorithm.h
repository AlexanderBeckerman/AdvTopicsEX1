#pragma once
#include "../common/abstract_algorithm.h"
#include "../common/sensors/sensors.h"
#include "../common/utils/relative_point.h"
#include "../common/utils/utils.h"
#include "expanding_map.h"
#include <optional>
#include <stack>
#include <unordered_set>

class SmartAlgorithm : public AbstractAlgorithm {
    // Configuration.
  protected:
    const DirtSensor *dirt_sensor;
    const WallsSensor *wall_sensor;
    const BatteryMeter *battery_sensor;
    size_t max_steps;
    size_t max_battery = 0;
    RelativePoint robot_location;

    // DFS state
    std::unordered_set<RelativePoint, RelativePointKeyHash> cleaned;
    std::unordered_set<RelativePoint, RelativePointKeyHash> visited;
    std::stack<Direction> direction_stack;

    std::optional<std::stack<Direction>> predetermined_path = std::nullopt;
    std::optional<RelativePoint> last_return_point = std::nullopt;
    size_t steps_left = 0;

    bool isValidMove(const Direction &direction) const {
        if (wall_sensor->isWall(direction))
            return false;

        // Not facing a wall.
        auto new_point = robot_location + direction;
        return cleaned.find(new_point) == cleaned.end();
    }

    void startReturn();

    Step moveDirection(const Direction &dir);

  public:
    ~SmartAlgorithm() override = default;
    SmartAlgorithm(SmartAlgorithm &) = delete;

    SmartAlgorithm() {
        max_steps = 0;
        steps_left = 0;
        robot_location = {0, 0};
        cleaned.insert(robot_location);
        visited.insert(robot_location);
    }
    void setMaxSteps(std::size_t maxSteps) override {
        this->max_steps = maxSteps;
        this->steps_left = maxSteps;
    }
    void setWallsSensor(const WallsSensor &ws) override {
        this->wall_sensor = &ws;
    }
    void setDirtSensor(const DirtSensor &ds) override {
        this->dirt_sensor = &ds;
    }
    void setBatteryMeter(const BatteryMeter &bm) override {
        this->max_battery = bm.getBatteryState();
        this->battery_sensor = &bm;
    }

    Step nextStep() override;
};