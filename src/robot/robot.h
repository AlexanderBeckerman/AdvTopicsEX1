#pragma once
#include "config.h"
#include "relative_point.h"
#include "sensors.h"
#include "smart_algorithm.h"
#include "step_info.h"
#include "stupid_algorithm.h"
#include "utils.h"

class StupidAlgorithm;

class Robot {
  protected:
    ConfigInfo config;

  private:
    ConcreteWallSensor wall_sensor;
    ConcreteBatteryMeter battery_sensor;
    ConcreteDirtSensor dirt_sensor;
    RelativePoint location;
    size_t curr_steps = 0;
    size_t exit_cond;

    void clean();
    bool canContinue();
    void logStep();

  public:
    Robot(ConfigInfo &&cfg) noexcept
        : config(std::move(cfg)),
          wall_sensor(config.getLayout(), config.charging_station),
          battery_sensor(config.max_battery_steps, config.max_battery_steps),
          dirt_sensor(config.getLayout(), config.charging_station),
          location({0, 0}) {}
    Robot(ConfigInfo &cfg)
        : config(cfg), wall_sensor(config.getLayout(), config.charging_station),
          battery_sensor(config.max_battery_steps, config.max_battery_steps),
          dirt_sensor(config.getLayout(), config.charging_station),
          location({0, 0}) {}

    Robot(const Robot &other) = delete;

    void move(const Direction direction);
    void step(const Step next_step);
    void start(SmartAlgorithm &algorithm);
    void debug() const {
        LOG(INFO) << "Robot at: " << location << std::endl;
        LOG(INFO) << "Battery level: " << battery_sensor.getBatteryState() << ""
                  << std::endl;
        LOG(INFO) << "Max steps: " << config.max_steps << "" << std::endl;
    }
    RelativePoint getLocation() const { return location; }
    const ConcreteWallSensor &getWallSensor() const { return wall_sensor; }
    const ConcreteDirtSensor &getDirtSensor() const { return dirt_sensor; }
    const ConcreteBatteryMeter &getBatterySensor() { return battery_sensor; }
    void dumpStepsInfo(const std::string &output_file) const;
    void serializeAndDumpSteps(const std::string &output_file) const;
    friend ConcreteWallSensor;
    friend ConcreteDirtSensor;
    friend ConcreteBatteryMeter;
    friend class AlgorithmTest;
    friend class RobotTest;
    friend class ExpandingMapTest;
    std::vector<StepInfo> steps_info;
};
