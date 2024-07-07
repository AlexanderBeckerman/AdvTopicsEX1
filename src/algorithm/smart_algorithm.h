#pragma once
#include "abstract_algorithm.h"
#include "config.h"
#include "expanding_map.h"
#include "relative_point.h"
#include "sensors.h"
#include "utils.h"

class SmartAlgorithm : public AbstractAlgorithm {

    std::shared_ptr<ConcreteDirtSensor>
        dirt_sensor; // Had to use shared ptr so we can have an empty
                     // constructor or else I got an error.
    std::shared_ptr<ConcreteWallSensor> wall_sensor;
    std::shared_ptr<ConcreteBatteryMeter> battery_sensor;
    ExpandingMap map;
    size_t max_steps;
    RelativePoint robot_location;

  public:
    SmartAlgorithm() = default;
    void setMaxSteps(std::size_t maxSteps) override {
        this->max_steps = maxSteps;
    }
    void setWallsSensor(const WallsSensor &ws) override {
        // Had to do this weird stuff to convert from abstract to concrete while
        // using a shared ptr.
        const ConcreteWallSensor *cws =
            dynamic_cast<const ConcreteWallSensor *>(&ws);
        this->wall_sensor = std::make_shared<ConcreteWallSensor>(*cws);
    }
    void setDirtSensor(const DirtSensor &ds) override {
        const ConcreteDirtSensor *cds =
            dynamic_cast<const ConcreteDirtSensor *>(&ds);
        this->dirt_sensor = std::make_shared<ConcreteDirtSensor>(*cds);
    }
    void setBatteryMeter(const BatteryMeter &bm) override {
        const ConcreteBatteryMeter *cbm =
            dynamic_cast<const ConcreteBatteryMeter *>(&bm);
        this->battery_sensor = std::make_shared<ConcreteBatteryMeter>(*cbm);
    }
    Step nextStep() { return Step::Stay; }
};