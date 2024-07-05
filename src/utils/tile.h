#pragma once
#include "utils.h"

enum class TileType {
    WALL,
    CHARGING_STATION,
    FLOOR,
};

class Tile {
    TileType type;
    std::optional<size_t> dirt_level;

  public:
    Tile(size_t dirt_level) : type(TileType::FLOOR), dirt_level(dirt_level) {}

    Tile(TileType type) : type(type), dirt_level(std::nullopt) {}

    void Clean() {
        if (dirt_level.has_value()) {
            dirt_level = safeSubtractOne(dirt_level.value());
        } else {
            throw std::invalid_argument("Cannot clean a non-floor tile.");
        }
    }

    TileType getType() const { return type; }

    size_t getDirtLevel() const {
        if (dirt_level.has_value()) {
            return dirt_level.value();
        } else {
            return 0;
        }
    }

    std::string toString() const {
        switch (type) {
        case TileType::WALL:
            return "W";
        case TileType::CHARGING_STATION:
            return "C";
        case TileType::FLOOR:
            return std::format("{}", dirt_level.value());
        }

        throw std::invalid_argument("Invalid tile type.");
    }
};