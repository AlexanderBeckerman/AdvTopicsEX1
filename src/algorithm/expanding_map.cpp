#include "expanding_map.h"

std::optional<std::reference_wrapper<Tile>>
ExpandingMap::getTile(const RelativePoint &loc) {
    auto search = data.find(loc);
    if (search == data.end()) {
        LOG(ERROR) << "No tile at location: " << loc << "" << std::endl;
        return std::nullopt; // Temoorary return.
    }
    return search->second;
}
