#include "expanding_map.h"

void ExpandingMap::addTile(const Coordinate loc, Tile &tile)
{
    auto search = data.find(loc);
    if (search == data.end())
    {
        data.insert({loc, tile});
    }
}

Tile &ExpandingMap::getTile(const Coordinate &loc)
{
    auto search = data.find(loc);
    if (search == data.end())
    {
        std::cerr << "No tile at location: " << loc << std::endl;
        return search->second; // Temoorary return.
    }
    return search->second;
}
