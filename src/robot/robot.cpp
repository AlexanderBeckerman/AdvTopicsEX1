#include "robot.h"
#include "config.h"
#include <cmath>
#include <algorithm>
#include <iostream>

void Robot::move(Direction direction)
{
    // This function will move the robot in the given direction
    auto curr_location = this->location;
    if (curr_location.isChargingStation())
        this->battery_sensor.stopCharging();

    std::cout << "Moving: " << direction << std::endl; 
    std::cout << "Current location in grid: " << curr_location << std::endl;
    this->location = curr_location + direction;
    std::cout << "Battery level: " << this->battery_sensor.batteryLevel() << std::endl;
    std::cout << "New location: " << this->location << std::endl;
    this->printLayout();
    this->battery_sensor.decreaseCharge();
}

void Robot::step()
{
    Direction direction = algorithm.nextMove();
    if (direction == Direction::STAY)
    {
        if (this->location.isChargingStation())
            this->battery_sensor.chargeBattery();
        else
            this->clean();
    } else {
        this->move(direction);
    }

    this->curr_steps++;
}

void Robot::clean()
{
    Tile &t = this->dirt_sensor.getCurrentTile();
    t.setDirtLevel(t.getDirtLevel() - 1);
    this->battery_sensor.decreaseCharge();

    // TODO(Ohad): log + output. printing for now
    std::cout << "Cleaned: " << this->location << std::endl;
    std::cout << "Battery level: " << this->battery_sensor.batteryLevel() << std::endl;

}

void Robot::start(){
    // This function will start the robot and make it clean the map
    while (canContinue())
    {
        this->step();
    }
}

void Robot::printLayout()
{
    std::cout << "--- LAYOUT INFORMATION (INSIDE ROBOT) ---" << std::endl;
    for (int i = 0; i < static_cast<int>(this->config.getLayout()->size()); i++)
    {
        for (int j = 0; j < static_cast<int>(this->config.getLayout()->at(i).size()); j++)
        {
            std::cout << (*this->config.getLayout())[i][j].getDirtLevel() << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-----------" << std::endl;
    // std::cout << "--- LAYOUT INFORMATION (INSIDE CONFIG) ---" << std::endl;
    // config.print();

}

bool Robot::canContinue()
{
    bool cleaned_all = this->location.isChargingStation() && this->config.getAmountToClean() == 0;
    bool stuck = !this->location.isChargingStation() && this->battery_sensor.batteryLevel() <= 0;
    bool still_have_steps = this->curr_steps < this->config.getMaxSteps();
    if (cleaned_all){
        std::cout << "Cleaned all and at charging station, exiting..." << std::endl;
    }
    else if(stuck){
        std::cout << "Stuck and battery is empty, exiting..." << std::endl;
    }
    else if (!still_have_steps){
        std::cout << "Reached max steps allowed, exiting..." << std::endl;
    }
    return still_have_steps && !cleaned_all && !stuck;
}

