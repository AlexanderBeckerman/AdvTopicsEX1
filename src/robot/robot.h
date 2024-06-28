#pragma once
#include "utils.h"
#include "config.h"
#include "sensors.h"
#include "algorithm.h"

class Algorithm;

class Robot
{
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
    bool canContinue();
    void logStep();

public:
    Robot(ConfigInfo &cfg) : config(cfg), 
                             wall_sensor(cfg.getLayout(), *this),
                             battery_sensor(config.max_battery_steps,config.max_battery_steps),
                             dirt_sensor(cfg.getLayout(), *this),
                             location({0, 0}),
                             algorithm(dirt_sensor, wall_sensor, battery_sensor) { }

    void move(const Direction direction);
    void step();
    void start();
    void debug()
    {
        LOG(INFO) << "Robot at: " << location << "" << std::endl;
        LOG(INFO) << "Battery level: " << battery_sensor.batteryLevel() << "" << std::endl;
        LOG(INFO) << "Max steps: " << config.max_steps << "" << std::endl;
    }
    void printLayout();
    Location getLocation() const { return location; }
    WallSensor &getWallSensor() { return wall_sensor; }
    DirtSensor &getDirtSensor() { return dirt_sensor; }
    BatterySensor &getBatterySensor() { return battery_sensor; }
    void dumpStepsInfo(const std::string &output_file);
    void serializeAndDumpSteps(const std::string &output_file);
    friend WallSensor;
    friend DirtSensor;
    friend BatterySensor;
    friend class AlgorithmTest;
    friend class RobotTest;
    friend class ExpandingMapTest;
    std::vector<StepInfo> steps_info;
};
