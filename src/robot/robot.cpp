#include "robot.h"
#include "config.h"

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
        return;
    }
    Direction d = algorithm.nextMove();
    this->move(d);
    this->battery_level--;
}

void Robot::clean()
{
    if (this->battery_level == 0)
    {
        // If the battery is empty, do nothing
        return;
    }
    // This function will clean the current location
    this->config.clean(this->location);
    this->battery_level--;
    // TODO(Ohad): log + output. printing for now
    std::cout << "Cleaned: " << this->location << std::endl;
}

void Robot::start()
{
    // This function will start the robot and make it clean the map
    while (true)
    {
        this->move();
        this->clean();
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