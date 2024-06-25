#pragma once
#include "utils.h"
#include "config.h"
#include "sensors.h"
#include "algorithm.h"

class Algorithm;

class Robot
{

    WallSensor wall_sensor;
    DirtSensor dirt_sensor;
    BatterySensor battery_sensor;

    size_t battery_level;
    size_t max_battery_level;
    int curr_steps = 0;
    int steps_charging = 0;

    Algorithm algorithm;
    Location location;

    protected: ConfigInfo &config;

    void clean();
    bool canContinue();

public:
    Robot(ConfigInfo &cfg) : config(cfg), wall_sensor(cfg.getLayout(), *this),
                             dirt_sensor(cfg.getLayout(), *this),
                             battery_sensor(*this),
                             battery_level(cfg.getMaxBatterySteps()),
                             max_battery_level(cfg.getMaxBatterySteps()),
                             location({0, 0}),
                             algorithm(dirt_sensor, wall_sensor, battery_sensor) {}

    void move(Direction direction);
    void move();
    void start();
    void debug()
    {
        std::cout << "Robot at: " << location << std::endl;
        std::cout << "Battery level: " << battery_sensor.BatteryLevel() << std::endl;
        std::cout << "Max steps: " << config.max_steps << std::endl;
    }
    void printLayout();
    size_t getBatteryLevel() const { return battery_level; }
    size_t getMaxBatteryLevel() const { return max_battery_level; }
    Location getLocation() const { return location; }
    WallSensor &getWallSensor() { return wall_sensor; }
    DirtSensor &getDirtSensor() { return dirt_sensor; }
    BatterySensor &getBatterySensor() { return battery_sensor; }
    friend WallSensor;
    friend DirtSensor;
};
