#pragma once
#include "utils.h"
#include <unordered_map>
#include <optional>
#include "tile.h"

class ExpandingMap
{
    std::unordered_map<Coordinate, Tile, LocationKeyHash, LocationKeyEqual> data;
    Coordinate charging_station;

public:
    ExpandingMap() : charging_station({0, 0}){}; 
    void addTile(const Coordinate loc, Tile &tile);
    std::optional<std::reference_wrapper<Tile>> getTile(const Coordinate &loc);
};