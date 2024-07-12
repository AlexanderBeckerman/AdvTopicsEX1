#pragma once
#include "abstract_algorithm.h"
#include "config.h"
#include "expanding_map.h"
#include "relative_point.h"
#include "sensors.h"
#include "utils.h"

class SmartAlgorithm : public AbstractAlgorithm {

    std::shared_ptr<DirtSensor> dirt_sensor;
    std::shared_ptr<WallsSensor> wall_sensor;
    std::shared_ptr<BatteryMeter> battery_sensor;
    ExpandingMap map;
    size_t max_steps;
    RelativePoint robot_location;

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
    Step nextStep() { return Step::Stay; }
};