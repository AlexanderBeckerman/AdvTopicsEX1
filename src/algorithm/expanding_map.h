#pragma once
#include "../common/utils/relative_point.h"
#include "../common/utils/tile.h"
#include "../common/utils/utils.h"
#include <optional>
#include <unordered_map>

class ExpandingMap {
    std::unordered_map<RelativePoint, Tile, RelativePointKeyHash, PointKeyEqual>
        data;

  public:
    ExpandingMap() : data() {};
    void addFloorTile(const RelativePoint &loc, size_t dirt_level) {
        data.insert({loc, Tile(dirt_level)});
    }
    void addWallTile(const RelativePoint &loc) {
        data.insert({loc, Tile(TileType::WALL)});
    }
    void addChargingStationTile(const RelativePoint &loc) {
        data.insert({loc, Tile(TileType::CHARGING_STATION)});
    }

    std::optional<std::reference_wrapper<Tile>>
    getTile(const RelativePoint &loc);
};