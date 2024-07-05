#pragma once
#include "utils.h"
#include <cstddef>
#include <functional>

struct RelativePoint {
    int x, y;

    RelativePoint operator+(const RelativePoint &rhs) const {
        return RelativePoint{x + rhs.x, y + rhs.y};
    }

    bool operator==(const RelativePoint &rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    RelativePoint operator+(const Direction rhs) const {
        switch (rhs) {
        case Direction::UP:
            return RelativePoint{x, y + 1};
        case Direction::DOWN:
            return RelativePoint{x, y - 1};
        case Direction::LEFT:
            return RelativePoint{x - 1, y};
        case Direction::RIGHT:
            return RelativePoint{x + 1, y};
        default:
            return *this;
        }
    }

    bool isChargingStation() const { return x == 0 && y == 0; }

    std::string to_string() const {
        return std::to_string(x) + "," + std::to_string(y);
    }

    friend std::ostream &operator<<(std::ostream &out, const RelativePoint &p) {
        out << p.to_string();
        return out;
    }
};

struct PointKeyHash {
    std::size_t operator()(const RelativePoint &k) const {
        return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1);
    }
};

struct PointKeyEqual {
    bool operator()(const RelativePoint &lhs, const RelativePoint &rhs) const {
        return lhs == rhs;
    }
};