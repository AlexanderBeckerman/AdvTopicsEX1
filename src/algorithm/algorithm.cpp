#include "algorithm.h"

Direction selectDirection(const std::vector<Direction> &possible_directions)
{
    if (possible_directions.empty())
    {
        std::cerr << "No possible direction." << std::endl;
        return Direction::STAY;
    }

    // Randomly select a direction from the possible directions.
    // TODO: Implement a better selection algorithm.
    return possible_directions[rand() % possible_directions.size()];
} 

Direction Algorithm::nextMove()
{
    if (notEnoughBattery()) 
        return returnToChargingStation();

    // Decide the next move.
    Direction next_direction = selectDirection(getPossibleDirections());
    Location next_location = this->robot_location + next_direction;
    if (next_location.isChargingStation())
    { 
        // Reset the path if we are at the charging station.
        path = std::stack<Direction>(); 
    } else {
        // Cache the path. TODO(Sasha): Implement a pathing algorithm and deperacte this.
        path.push(next_direction);         
    }

    // Update the map with the current tile we discovered.
    updateMap(Direction::STAY); 

    // Update the current location of the robot 
    this->robot_location = next_location; 
    return next_direction;
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
    return this->path.size() == this->battery_sensor.batteryLevel()-1;
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
    this->robot_location = this->robot_location + d;
}

void Algorithm::updateMap(Direction d)
{
    if (d != Direction::STAY)
    {
        Location l = this->robot_location + d;
        Tile t = this->wall_sensor.getWallTile(d);
        this->map.addTile(l, t);
    }
    else
    {
        Tile &t = this->dirt_sensor.getCurrentTile();
        this->map.addTile(this->robot_location, t);
    }
}

std::vector<Direction> Algorithm::getPossibleDirections()
{
    std::vector<Direction> possible_directions;
    // if (this->curr_pos.isChargingStation() && this->battery_sensor.BatteryLevel() < this->battery_sensor.getMaxBatteryLevel())
    // {
    //     possible_directions.push_back(Direction::STAY);
    //     return possible_directions;
    // } 
    Direction directions[] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
    for (auto d : directions)
    {
        if (this->wall_sensor.isWall(d))
            updateMap(d);
        else
            possible_directions.push_back(d);
    }

    // Check if we are on a clean tile and if not then we add STAY to the possible directions.
    if (this->dirt_sensor.isDirty() || this->robot_location.isChargingStation() && 
            this->battery_sensor.batteryLevel() < this->battery_sensor.getCapacity())
    {
        possible_directions.push_back(Direction::STAY);
    }

    return possible_directions;
}