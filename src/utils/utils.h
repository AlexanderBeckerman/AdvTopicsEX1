#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


struct Position {
    int y; // Will represent the row idx in the map.
    int x; // Will represent the col idx in the map.
};

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

