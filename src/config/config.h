#pragma once
#include "utils.h"
#include "tile.h"

class Robot;

typedef std::vector<std::vector<Tile>> TileLayout;
struct LayoutPoint : public Coordinate {
    friend LayoutPoint operator+(const LayoutPoint &lhs, const Coordinate &rhs){
        Coordinate cord = lhs + rhs;
        return LayoutPoint{cord.x, cord.y};
    }
};

class ConfigInfo
{
    std::shared_ptr<TileLayout> topograhpy_data;
    LayoutPoint charging_station;
    size_t max_battery_steps;
    size_t max_steps;
    size_t amount_to_clean = 0;

    friend Robot;

public:
    ConfigInfo(std::string path);
    void draw();
    void setValueAt(LayoutPoint position, int value);
    int getValueAt(LayoutPoint position) const;
    void clean(LayoutPoint position);
    void print() const;
    void setAmountToClean(size_t amount) { amount_to_clean = amount; }
    std::shared_ptr<TileLayout> getLayout() const;
    size_t getMaxBatterySteps() const { return max_battery_steps; }
    size_t getMaxSteps() const { return max_steps; }
    LayoutPoint getChargingStation() const { return charging_station; }
    size_t getAmountToClean() const { return amount_to_clean; }

private:
    bool checkInRange(LayoutPoint p) const;
};

inline Tile TileFromCode(LayoutPoint loc, int code)
{
    switch (code)
    {
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