#pragma once
#include "utils.h"
#include "tile.h"

class Robot;

typedef std::vector<std::vector<Tile>> TileLayout;

class ConfigInfo { 
    TileLayout topograhpy_data;
    Location charging_station;
    size_t max_battery_steps;
    size_t max_steps;

    friend Robot;
    
public:
    ConfigInfo(std::string path);
    void draw();
    void setValueAt(Location position, int value);
    int getValueAt(Location position) const;
    void clean(Location position);
    void print() const;
    TileLayout& getData();
    size_t getMaxBatterySteps() const { return max_battery_steps; }
    size_t getMaxSteps() const { return max_steps; }
    
private:
    bool checkInRange(Location p) const;
};


inline Tile TileFromCode(Location loc, int code){
    switch (code){
        case -2:
            return Tile(loc, TileType::WALL);
        case -1:
            return Tile(loc, TileType::CHARGING_STATION);
        default:
            return Tile(loc, code);
    }
}

/* 
The input file will be in the following format:
The first line will hold the max_battery_steps and max_steps respectively separated by a space.
After that the map will be represented by a matrix of integers.
Each integer will be separated by a space.
-2 will represent a wall.
-1 will represent a charging station.
0-9 will represent the cleanliness of the cell, 0 being the cleanest.
*/