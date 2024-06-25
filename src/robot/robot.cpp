#include "robot.h"
#include "config.h"
#include <cmath>

void Robot::move(Direction direction)
{
    std::cout << "Before move: " << this->location << std::endl;
    // This function will move the robot in the given direction
    this->location = this->location + direction;
    // TODO(Ohad): log + output. printing for now
    std::cout << "Moved to: " << this->location << std::endl;
}

void Robot::move()
{
    if (this->battery_level == 0)
    {
        // If the battery is empty, do nothing
        std::cerr << "Battery is empty, can't clean\n"
                  << std::endl;
        return;
    }

    Direction d = algorithm.nextMove();
    if (d == Direction::STAY)
    {
        if (this->location.isChargingStation())
        {
            this->steps_charging++; // How many steps in a row we are charging
            this->battery_level++; // Keep battery level the same, so when we move we calclate new battery level using their formula.
        }
        else{
            this->clean();
        }
    }
    if (this->location.isChargingStation() && d != Direction::STAY)
    {
        this->battery_level = this->battery_level + (this->steps_charging)*std::round((this->config.getMaxBatterySteps()/20));
    }
    else{
        this->steps_charging = 0;
    }
    this->move(d);
    this->curr_steps++;
    this->battery_level--;
}

void Robot::clean()
{
    // This function will clean the current location
    Tile &t = this->dirt_sensor.getCurrentTile();
    t.setDirtLevel(t.getDirtLevel() - 1);

    // TODO(Ohad): log + output. printing for now
    std::cout << "Cleaned: " << this->location << std::endl;
}

void Robot::start(){
    // This function will start the robot and make it clean the map
    while (canContinue())
    {
        this->move();
    }
}

void Robot::printLayout()
{
    for (int i = 0; i < this->config.getLayout()->size(); i++)
    {
        for (int j = 0; j < this->config.getLayout()->at(i).size(); j++)
        {
            std::cout << (*this->config.getLayout())[i][j].getDirtLevel() << " ";
        }
        std::cout << std::endl;
    }
}

bool Robot::canContinue()
{
    // This function will check if the robot can continue cleaning
    bool cleaned_all = this->location.isChargingStation() && this->config.getAmountToClean() == 0;
    bool stuck = !this->location.isChargingStation() && this->battery_level == 0;
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

