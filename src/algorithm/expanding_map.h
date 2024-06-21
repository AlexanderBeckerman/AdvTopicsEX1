#pragma once
#include "utils.h"
#include <unordered_map>
#include <tile.h>

class ExpandingMap
{
    std::unordered_map<Location, Tile, LocationKeyHash, LocationKeyEqual> data;
    Location charging_station;

public:
    ExpandingMap() : charging_station({0, 0}){}; 
    void addTile(const Location loc, Tile &tile);
    Tile &getTile(const Location &loc);
};