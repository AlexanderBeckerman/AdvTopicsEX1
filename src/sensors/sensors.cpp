#include "sensors.h"
#include "tile.h"

// This function will check if the current tile is dirty.
bool ConcreteDirtSensor::isDirty() const {
    return this->getCurrentTile().getDirtLevel() > 0;
}

int ConcreteDirtSensor::dirtLevel() const {
    // This function will return the dirt level of the current tile
    return this->getCurrentTile().getDirtLevel();
}

bool ConcreteWallSensor::isWall(const Direction direction) const {
    size_t row = location.row;
    size_t col = location.col;
    switch (direction) {
    case Direction::North:
        if (row == 0) {
            return true;
        }
        break;
    case Direction::South:
        if (row == layout->size() - 1) {
            return true;
        }
        break;
    case Direction::West:
        if (col == 0) {
            return true;
        }
        break;
    case Direction::East:
        if (col == (*layout)[row].size() - 1) {
            return true;
        }
        break;
    default:
        return false;
    };
    auto layout_point = location + direction;
    return (*layout)[layout_point.row][layout_point.col].getType() ==
           TileType::WALL;
}
