#include "algorithm.h"

Direction Algorithm::nextMove()
{
    if (notEnoughBattery()) // TODO: Maybe add a check so that if we are in the charging station we want to stay a bit and not move randomly.
    {
        return returnToChargingStation();
    }
    std::vector<Direction> possible_directions = getPossibleDirections();
    if (possible_directions.size() == 0)
    {
        std::cout << "No possible direction." << std::endl;
        return Direction::STAY;
    }
    Direction rand_direction = possible_directions[rand() % possible_directions.size()];
    if ((this->curr_pos + rand_direction) != Location(0, 0))
    {   // If we are charging we don't want it to count as a move. Need to think about this more not sure this works as intended.
        path.push(rand_direction); // Remember the path we took so we can return to the charging station.         
    }
    updateMap(rand_direction); // Update the map with the new tile we discovered.
    setLocation(rand_direction); // Update the current location of the robot (independent of the location of the robot in Robot class).
    return rand_direction;
}

bool Algorithm::shouldMove(Direction d) // This checks if the robot should move in the given direction
{

    if (this->dirt_sensor.isDirty() && d == Direction::STAY)
    {
        // We don't want to stay on a clean tile.
        return true;
    }
    return !this->wall_sensor.isWall(d); // Move if there is no wall in the given direction.
}

bool Algorithm::notEnoughBattery()
{
    return this->path.size() == this->battery_sensor.BatteryLevel();
}

Direction Algorithm::returnToChargingStation()
{
    // Returns the direction opposite to the one at the top of the stack so we reach the charging station.
    Direction last_dir = this->path.top();
    this->path.pop();

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

std::vector<Direction> Algorithm::getPossibleDirections()
{
    std::vector<Direction> possible_directions;
    if (this->curr_pos == Location(0, 0) && this->battery_sensor.BatteryLevel() < this->battery_sensor.getMaxBatteryLevel())
    {
        possible_directions.push_back(Direction::STAY);
        return;
    } 
    for (int i = 0; i < 3; i++)
    {
        Direction d = static_cast<Direction>(i);
        if (!this->wall_sensor.isWall(d))
        {
            possible_directions.push_back(d);
        }
    }
    // Check if we are on a clean tile and if not then we add STAY to the possible directions.
    if (this->dirt_sensor.isDirty())
    {
        possible_directions.push_back(Direction::STAY);
    }
    

    return possible_directions;
}