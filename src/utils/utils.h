#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ostream>
#include <memory>

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Location {
    int y; // Will represent the row idx in the map.
    int x; // Will represent the col idx in the map.

    friend std::ostream& operator<<(std::ostream& os, const Location& loc) {
        os << "(" << loc.y << "," << loc.x << ")";
        return os;
    }

    friend Location operator +(const Location& lhs, Direction rhs) {
        switch (rhs) {
            case Direction::UP:
                return Location{lhs.y + 1, lhs.x};
            case Direction::DOWN:
                return Location{lhs.y - 1, lhs.x};
            case Direction::LEFT:
                return Location{lhs.y, lhs.x - 1};
            case Direction::RIGHT:
                return Location{lhs.y, lhs.x + 1};
        }
        return lhs;
    }
};

// Hash function for Location struct.
struct LocationKeyHash {
    std::size_t operator()(const Location& k) const {
        return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1);
    }
};

// Compare function for Location struct.
struct LocationKeyEqual {
    bool operator()(const Location& lhs, const Location& rhs) const {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

