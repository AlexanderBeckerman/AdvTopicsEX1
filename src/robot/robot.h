#pragma once
#include "utils.h"
#include "config.h"
#include "algorithm.h"
#include "sensors.h"

class Algorithm;

class Robot
{
    ConfigInfo &config;
    Location charging_station;
    WallSensor wall_sensor;
    DirtSensor dirt_sensor;
    BatterySensor battery_sensor;
    size_t battery_level;

    Algorithm algorithm;
    Location location;

    void clean();

public:
    Robot(ConfigInfo &cfg) : config(cfg), wall_sensor(cfg.getLayout(), *this), 
        dirt_sensor(cfg.getLayout(), *this), 
        battery_sensor(*this), 
        battery_level(cfg.getMaxBatterySteps()), 
        location(cfg.getChargingStation()),
                             charging_station(cfg.getChargingStation()) {}

    void move(Direction direction);
    void move();
    void start();
    void debug()
    {
        std::cout << "Robot at: " << location << std::endl;
        std::cout << "Battery level: " << battery_sensor.BatteryLevel() << std::endl;
        std::cout << "Cleaned: " << charging_station << std::endl;
        std::cout << "Max steps: " << config.max_steps << std::endl;
    }
    void printLayout();
    size_t getBatteryLevel() const { return battery_level; }
    Location getLocation() const { return location; }
    WallSensor &getWallSensor() { return wall_sensor; }
    DirtSensor &getDirtSensor() { return dirt_sensor; }
    BatterySensor &getBatterySensor() { return battery_sensor; }
};
