#include "expanding_map.h"

void ExpandingMap::addTile(const Coordinate loc, Tile &tile) {
    auto search = data.find(loc);
    if (search == data.end()) {
        data.insert({loc, tile});
    }
}

std::optional<std::reference_wrapper<Tile>>
ExpandingMap::getTile(const Coordinate &loc) {
    auto search = data.find(loc);
    if (search == data.end()) {
        LOG(ERROR) << "No tile at location: " << loc << "" << std::endl;
        return std::nullopt; // Temoorary return.
    }
    return search->second;
}
