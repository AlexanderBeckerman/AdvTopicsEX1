#pragma once
#include "utils.h"
#include "config.h"
#include "algorithm.h"
#include "sensors.h"

class Algorithm;

class Robot {
    ConfigInfo config;
    Location charging_station; 
    WallSensor wall_sensor;
    DirtSensor dirt_sensor;
    BatterySensor battery_sensor;
    size_t battery_level;

    Algorithm algorithm;
    Location location;

    void move(Direction direction);
    void clean();
public:
    Robot(ConfigInfo& cfg) : config(cfg), wall_sensor(std::make_shared<TileLayout>(cfg.getData()), *this)
        ,dirt_sensor(std::make_shared<TileLayout>(cfg.getData()), *this), battery_sensor(*this), battery_level(cfg.getMaxBatterySteps())  {}

    void move();
    void start();
    void debug() {
        std::cout << "Robot at: " << location << std::endl;
        std::cout << "Battery level: " << battery_sensor.BatteryLevel() << std::endl;
        std::cout << "Cleaned: " << charging_station << std::endl;
        std::cout << "Max steps: " << config.max_steps << std::endl;
    }
    size_t getBatteryLevel() const { return battery_level; }
    Location getLocation() const { return location; }
    
};



