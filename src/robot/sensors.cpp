#include "sensors.h"
#include "robot.h"
#include "tile.h"

// This function will check if the current tile is dirty.
bool DirtSensor::isDirty() const {
    return this->getCurrentTile().getDirtLevel() > 0;
}

int DirtSensor::DirtLevel() const {
    // This function will return the dirt level of the current tile
    return this->getCurrentTile().getDirtLevel();
}

bool WallSensor::isWall(const Direction direction) const {
    size_t row = location.row;
    size_t col = location.col;
    switch (direction) {
    case Direction::UP:
        if (row == 0) {
            return true;
        }
        break;
    case Direction::DOWN:
        if (row == layout->size() - 1) {
            return true;
        }
        break;
    case Direction::LEFT:
        if (col == 0) {
            return true;
        }
        break;
    case Direction::RIGHT:
        if (col == (*layout)[row].size() - 1) {
            return true;
        }
        break;
    case Direction::STAY:
        return false;
    };
    auto layout_point = location + direction;
    return (*layout)[layout_point.row][layout_point.col].getType() ==
           TileType::WALL;
}
