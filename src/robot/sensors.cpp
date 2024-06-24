#include "robot.h"
#include "sensors.h"
#include "tile.h"

bool DirtSensor::isDirty()
{
    // This function will check if the current tile is dirty
    std::cout << "ROBOT LOCATION: " << robot.getLocation() << std::endl;
    Tile t = (*layout)[robot.getLocation().y][robot.getLocation().x];
    return t.getType() == TileType::FLOOR && t.getDirtLevel() > 0;
}

int DirtSensor::DirtLevel()
{
    // This function will return the dirt level of the current tile
    std::cout << "ROBOT LOCATION: " << robot.getLocation() << std::endl;
    Tile t = (*layout)[robot.getLocation().y][robot.getLocation().x];
    std::cout << "--- LAYOUT INFORMATION ---" << std::endl;
    for (int i = 0; i < layout->size(); i++)
    {
        for (int j = 0; j < layout->at(i).size(); j++)
        {
            std::cout << (*layout)[i][j].getDirtLevel() << " ";
        }
        std::cout << std::endl;
    }
    return t.getDirtLevel();
}

bool WallSensor::isWall(Direction direction)
{
    // This function will check if there is a wall in the given direction
    Location loc = robot.getLocation() + direction;
    if (loc.x < 0 || loc.y < 0 || loc.x >= layout->at(0).size() || loc.y >= layout->size())
    {
        return true;
    }
    return (*layout)[loc.y][loc.x].getType() == TileType::WALL;
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
    return (*layout)[robot.getLocation().y][robot.getLocation().x];
}

Tile &WallSensor::getWallTile(Direction d)
{
    // This function will return the tile in the given direction
    Location loc = robot.getLocation() + d;
    return (*layout)[loc.y][loc.x];
}