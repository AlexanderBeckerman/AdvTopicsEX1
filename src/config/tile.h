#pragma once
#include "utils.h"

enum class TileType {
    WALL,
    CHARGING_STATION,
    FLOOR,
};

class Tile {
    Location loc; // Relative to 0,0 (e.g. charging station).
    TileType type;
    int dirt_level;

public:
    Tile(Location loc, int dirt_level) : loc(loc), type(TileType::FLOOR), dirt_level(dirt_level) {}
    Tile(Location loc, TileType type) : loc({0, 0}), type(type), dirt_level(-1) {
        if (type == TileType::FLOOR) {
            throw std::invalid_argument("Tile type cannot be FLOOR without dirt level.");
        }
    }
    Location getLocation() const { return loc; }
    TileType getType() const { return type; }
    int getDirtLevel() const { return dirt_level; } // TODO: Error when called on non-floor tiles.
    void setDirtLevel(int dirt_level) { this->dirt_level = dirt_level; }
    void setType(TileType type) { this->type = type; }
    void setLocation(Location loc) { this->loc = loc; }
    void print() const {
        switch (type) {
            case TileType::WALL:
                std::cout << "W";
                break;
            case TileType::CHARGING_STATION:
                std::cout << "C";
                break;
            case TileType::FLOOR:
                std::cout << dirt_level;
                break;
        }
        std::cout << " ";
    }
};

