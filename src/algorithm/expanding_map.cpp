#include "expanding_map.h"

void ExpandingMap::addTile(const Location loc, Tile &tile)
{
    auto search = data.find(loc);
    if (search == data.end())
    {
        data.insert({loc, tile});
    }
    else
    {
        std::cerr << "Tile already exists at location: " << loc << std::endl;
    }
}

Tile &ExpandingMap::getTile(const Location &loc)
{
    auto search = data.find(loc);
    if (search == data.end())
    {
        std::cerr << "No tile at location: " << loc << std::endl;
        return search->second; // Temoorary return.
    }
    return search->second;
}
