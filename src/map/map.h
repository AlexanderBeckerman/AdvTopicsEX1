#pragma once
#include "utils.h"


class Robot;

class Map { 
    std::vector<std::vector<int>> data;
    Position charging_station;
    
public:
    Map(std::string path, Robot& robot);
    void draw();
    void setValueAt(Position position, int value);
    void clean(Position position);
    void print() const;
    std::vector<std::vector<int>>& getData();
    
private:
    bool checkInRange(Position p) const;
    std::vector<std::vector<int>> getMapFromFile(const std::string path, Robot& robot);
};



/* 
The input file will be in the following format:
The first line will hold the max_battery_steps and max_steps respectively separated by a space.
After that the map will be represented by a matrix of integers.
Each integer will be separated by a space.
-2 will represent a wall.
-1 will represent a charging station.
0-9 will represent the cleanliness of the cell, 0 being the cleanest.
*/