#include "expanding_map.h"

void ExpandingMap::AddTile(const Location& loc, const Tile& tile) {
    auto search = data.find(loc);
    if (search == data.end()) {
        data.insert({loc, tile});
    }
    else {
        std::cerr << "Tile already exists at location: " << loc << std::endl;
    }
}

Tile& ExpandingMap::GetTile(const Location& loc) {
    auto search = data.find(loc);
    if (search == data.end()) {
        std::cerr << "No tile at location: " << loc << std::endl;
        return search->second; // Temoorary return.
    }
    return search->second;
}

