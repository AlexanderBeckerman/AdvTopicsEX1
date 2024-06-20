#include "algorithm.h"

Direction Algorithm::nextMove()
{
    if (notEnoughBattery())
    {
        return returnToChargingStation();
    }
    Direction rand_direction = static_cast<Direction>(rand() % 5);
    while (!shouldClean(rand_direction))
    {
        rand_direction = static_cast<Direction>(rand() % 5);
    }
    path.push(rand_direction);
    setLocation(rand_direction);
    // updateMap(rand_direction, ); TODO: Implement updateMap function.
    return rand_direction;
}

bool Algorithm::shouldClean(Direction d)
{
    if (this->dirt_sensor.isDirty() && d == Direction::STAY)
    {
        return true;
    }
    return !this->wall_sensor.isWall(d);
}

bool Algorithm::notEnoughBattery()
{
    return this->path.size() == this->battery_sensor.BatteryLevel();
}

Direction Algorithm::returnToChargingStation()
{
    Direction last_dir = this->path.top();
    this->path.pop();
    if (last_dir == Direction::STAY)
    {
        return Direction::STAY;
    }
    Direction opposite_dir;
    switch (last_dir)
    {
    case Direction::UP:
        return Direction::DOWN;
    case Direction::DOWN:
        return Direction::UP;
    case Direction::LEFT:
        return Direction::RIGHT;
    case Direction::RIGHT:
        return Direction::LEFT;
    default:
        return Direction::STAY;
    }
}

void Algorithm::setLocation(Direction d)
{
    this->curr_pos = this->curr_pos + d;
}