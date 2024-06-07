#include <vector>
#include <string>
#include "utils.h"
#include "robot.h"

class Map { 
    std::vector<std::vector<int>> data;

    Map(std::vector<std::vector<int>> data) : data(data) {}
    Map(std::string path);
    void draw();
    int clean(Position position);
};
