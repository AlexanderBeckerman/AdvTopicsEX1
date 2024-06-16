#include "robot.h"
#include "sensors.h"

bool DirtSensor::isDirty() {
    // This function will check if the current tile is dirty
    Tile t = (*layout)[robot.getLocation().y][robot.getLocation().x];
    return t.getDirtLevel() > 0;
}

int DirtSensor::DirtLevel() {
    // This function will return the dirt level of the current tile
    Tile t = (*layout)[robot.getLocation().y][robot.getLocation().x];
    return t.getDirtLevel();
}

bool WallSensor::isWall(Direction direction) {
    // This function will check if there is a wall in the given direction
    Location loc = robot.getLocation() + direction;
    return (*layout)[loc.y][loc.x].getType() == TileType::WALL;
}

int BatterySensor::BatteryLevel() {
    // This function will return the battery level
    return robot.getBatteryLevel();
}