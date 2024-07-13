#pragma once
#include "relative_point.h"
#include "tile.h"
#include "expanding_map.h"
#include "utils.h"
#include <optional>
#include <unordered_map>
#include "graph.h"

class SmartMap {
    ExpandingMap tile_data;
    Graph graph;
    
public:
    SmartMap();
    ~SmartMap() = default;
    void addFloorTile(const RelativePoint &loc, size_t dirt_level) {
        tile_data.addFloorTile(loc, dirt_level);
    }
    void addWallTile(const RelativePoint &loc) {
        tile_data.addWallTile(loc);
    }
    void addChargingStationTile(const RelativePoint &loc) {
        tile_data.addChargingStationTile(loc);
    }
};