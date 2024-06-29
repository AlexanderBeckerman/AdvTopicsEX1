#pragma once
#include "utils.h"

enum class TileType {
    WALL,
    CHARGING_STATION,
    FLOOR,
};

class Tile {
    Coordinate loc; // Topographical location.
    TileType type;
    int dirt_level;

  public:
    Tile(Coordinate loc, int dirt_level)
        : loc(loc), type(TileType::FLOOR), dirt_level(dirt_level) {}
    Tile(Coordinate loc, TileType type) : loc(loc), type(type) {
        if (type == TileType::FLOOR) {
            throw std::invalid_argument(
                "Tile type cannot be FLOOR without dirt level.");
        }
        type == TileType::WALL ? dirt_level = -2 : dirt_level = -1;
    }
    Coordinate getLocation() const { return loc; }
    TileType getType() const { return type; }
    int getDirtLevel() const {
        return dirt_level;
    } // TODO: Error when called on non-floor tiles.
    void setDirtLevel(int dirt_level) { this->dirt_level = dirt_level; }
    void setType(TileType type) { this->type = type; }
    void setLocation(Coordinate loc) { this->loc = loc; }
    void print() const {
        switch (type) {
        case TileType::WALL:
            LOG(INFO) << "W";
            break;
        case TileType::CHARGING_STATION:
            LOG(INFO) << "C";
            break;
        case TileType::FLOOR:
            LOG(INFO) << dirt_level;
            break;
        }
        LOG(INFO) << " ";
    }
};
