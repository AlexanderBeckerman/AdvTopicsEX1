#include "sensors.h"
#include "tile.h"
#include "robot.h"

bool DirtSensor::isDirty() const
{
    // This function will check if the current tile is dirty
    Coordinate charging_idx = (Coordinate)robot.config.getChargingStation();
    Coordinate robot_idx = (Coordinate)robot.getLocation();
    Coordinate point = {charging_idx.x + robot_idx.x, charging_idx.y - robot_idx.y};
    Tile t = (*layout)[point.y][point.x];
    
    return t.getType() == TileType::FLOOR && t.getDirtLevel() > 0;
}

int DirtSensor::DirtLevel() const
{
    // This function will return the dirt level of the current tile
    Coordinate charging_idx = (Coordinate)robot.config.getChargingStation();
    Coordinate robot_idx = (Coordinate)robot.getLocation();
    Coordinate point = {charging_idx.x + robot_idx.x, charging_idx.y - robot_idx.y};
    Tile t = (*layout)[point.y][point.x];
    return t.getDirtLevel();
}

bool WallSensor::isWall(const Direction direction) const
{
    // This function will check if there is a wall in the given direction
    Coordinate charging_idx = (Coordinate)robot.config.getChargingStation();
    Coordinate robot_idx = (Coordinate)robot.getLocation() + direction;
    Coordinate point = {charging_idx.x + robot_idx.x, charging_idx.y - robot_idx.y};
    if (point.x < 0 || point.y < 0 || point.x >= static_cast<int>(layout->at(0).size()) || point.y >= static_cast<int>(layout->size()))
    {
        return true;
    }
    return (*layout)[point.y][point.x].getType() == TileType::WALL;
}

Tile &DirtSensor::getCurrentTile()
{
    // This function will return the current tile
    Coordinate charging_idx = (Coordinate)robot.config.getChargingStation();
    Coordinate robot_idx = (Coordinate)robot.getLocation();
    Coordinate point = {charging_idx.x + robot_idx.x, charging_idx.y - robot_idx.y};
    return (*layout)[point.y][point.x];
}

Tile WallSensor::getWallTile(const Direction d) const
{
    // This function will return the tile in the given direction
    Coordinate charging_idx = (Coordinate)robot.config.getChargingStation();
    Coordinate robot_idx = (Coordinate)robot.getLocation() + d;
    Coordinate point = {charging_idx.x + robot_idx.x, charging_idx.y - robot_idx.y};
    if (point.x < 0 || point.y < 0 || point.x >= static_cast<int>(layout->at(0).size()) || point.y >= static_cast<int>(layout->size()))
    {
        return Tile(point, TileType::WALL);
    }
    return (*layout)[point.y][point.x];
}