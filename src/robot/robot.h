#pragma once
#include "algorithm.h"
#include "config.h"
#include "sensors.h"
#include "utils.h"

class Algorithm;

class Robot {
  protected:
    ConfigInfo config;

  private:
    WallSensor wall_sensor;
    BatterySensor battery_sensor;
    DirtSensor dirt_sensor;
    Location location;
    Algorithm algorithm;
    size_t curr_steps = 0;
    size_t exit_cond;

    void clean();
    bool canContinue() const;
    void logStep();

  public:
    Robot(ConfigInfo &&cfg)
        : config(std::move(cfg)), wall_sensor(config.getLayout(), *this),
          battery_sensor(config.max_battery_steps, config.max_battery_steps),
          dirt_sensor(config.getLayout(), *this), location({0, 0}),
          algorithm(dirt_sensor, wall_sensor, battery_sensor) {}
    Robot(ConfigInfo &cfg)
        : config(cfg), wall_sensor(config.getLayout(), *this),
          battery_sensor(config.max_battery_steps, config.max_battery_steps),
          dirt_sensor(config.getLayout(), *this), location({0, 0}),
          algorithm(dirt_sensor, wall_sensor, battery_sensor) {}

    Robot(const Robot &other) = delete;

    void move(const Direction direction);
    void step();
    void start();
    void debug() const {
        LOG(INFO) << "Robot at: " << location << "" << std::endl;
        LOG(INFO) << "Battery level: " << battery_sensor.batteryLevel() << ""
                  << std::endl;
        LOG(INFO) << "Max steps: " << config.max_steps << "" << std::endl;
    }
    void printLayout() const;
    Location getLocation() const { return location; }
    const WallSensor &getWallSensor() const { return wall_sensor; }
    const DirtSensor &getDirtSensor() const { return dirt_sensor; }
    const BatterySensor &getBatterySensor() { return battery_sensor; }
    void dumpStepsInfo(const std::string &output_file) const;
    void serializeAndDumpSteps(const std::string &output_file) const;
    friend WallSensor;
    friend DirtSensor;
    friend BatterySensor;
    friend class AlgorithmTest;
    friend class RobotTest;
    friend class ExpandingMapTest;
    std::vector<StepInfo> steps_info;
};
