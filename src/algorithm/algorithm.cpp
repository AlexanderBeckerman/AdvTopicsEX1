#include "algorithm.h"

Direction Algorithm::nextMove()
{
    if (notEnoughBattery())
    {
        return returnToChargingStation();
    }
    Direction rand_direction = static_cast<Direction>(rand() % 5);
    while (!shouldMove(rand_direction))
    {
        rand_direction = static_cast<Direction>(rand() % 5);
    }
    path.push(rand_direction);
    updateMap(rand_direction);
    setLocation(rand_direction);
    return rand_direction;
}

bool Algorithm::shouldMove(Direction d)
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

void Algorithm::updateMap(Direction d)
{
    if (this->wall_sensor.isWall(d))
    {
        Location l = this->curr_pos + d;
        Tile &t = this->wall_sensor.getWallTile(d);
        this->map.addTile(l, t);
    }
    else
    {
        Tile &t = this->dirt_sensor.getCurrentTile();
        this->map.addTile(this->curr_pos + d, t);
    }
}
