#include "robot.h"
#include "sensors.h"
#include "tile.h"

bool DirtSensor::isDirty()
{
    // This function will check if the current tile is dirty
    std::cout << "ROBOT LOCATION: " << robot.getLocation() << std::endl;
    Coordinate point = (Coordinate)robot.config.getChargingStation() + robot.getLocation();
    Tile t = (*layout)[-point.y][point.x];
    return t.getType() == TileType::FLOOR && t.getDirtLevel() > 0;
}

int DirtSensor::DirtLevel()
{
    // This function will return the dirt level of the current tile
    // std::cout << "ROBOT LOCATION: " << robot.getLocation() << std::endl;
    Coordinate point = (Coordinate)robot.config.getChargingStation() + robot.getLocation();
    Tile t = (*layout)[-point.y][point.x];
    // std::cout << "--- LAYOUT INFORMATION ---" << std::endl;
    // for (int i = 0; i < layout->size(); i++)
    // {
    //     for (int j = 0; j < layout->at(i).size(); j++)
    //     {
    //         std::cout << (*layout)[i][j].getDirtLevel() << " ";
    //     }
    //     std::cout << std::endl;
    // }
    return t.getDirtLevel();
}

bool WallSensor::isWall(Direction direction)
{
    // This function will check if there is a wall in the given direction
    Location new_loc = robot.getLocation() + direction; // make this location

    Coordinate point = (Coordinate)robot.config.getChargingStation() + new_loc;
    if (point.x < 0 || -point.y < 0 || point.x >= layout->at(0).size() || -point.y >= layout->size())
    {
        return true;
    }
    return (*layout)[-point.y][point.x].getType() == TileType::WALL;
}

size_t BatterySensor::BatteryLevel(){
    // This function will return the battery level
    return robot.getBatteryLevel();
}

size_t BatterySensor::getMaxBatteryLevel(){
    return robot.getMaxBatteryLevel();
}

Tile &DirtSensor::getCurrentTile()
{
    // This function will return the current tile
    Coordinate point = (Coordinate)robot.config.getChargingStation() + robot.getLocation();
    return (*layout)[-point.y][point.x];
}

Tile WallSensor::getWallTile(Direction d) const
{
    // This function will return the tile in the given direction
    Location new_loc = robot.getLocation() + d;
    Coordinate point = (Coordinate)robot.config.getChargingStation() + new_loc;
    if (point.x < 0 || -point.y < 0 || point.x >= layout->at(0).size() || -point.y >= layout->size())
    {
        return Tile(point, TileType::WALL);
    }
    return (*layout)[-point.y][point.x];
}