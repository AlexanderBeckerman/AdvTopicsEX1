#pragma once
#include "utils.h"
#include <unordered_map>
#include <tile.h>

class ExpandingMap {
    std::unordered_map<Location, Tile, LocationKeyHash, LocationKeyEqual> data;
    Location charging_station;

public:
    ExpandingMap(): charging_station({0, 0}) {};
    void AddTile(const Location& loc, const Tile& tile);
    Tile& GetTile(const Location& loc);
};