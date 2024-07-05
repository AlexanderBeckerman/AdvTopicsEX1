#pragma once
#include "utils.h"
#include <ostream>

struct LayoutPoint {
    size_t row;
    size_t col;

    std::string toString() const {
        return "(" + std::to_string(row) + "," + std::to_string(col) + ")";
    }

    friend std::ostream &operator<<(std::ostream &os, const LayoutPoint &loc) {
        os << loc.toString();
        return os;
    }

    friend bool operator==(const LayoutPoint &lhs, const LayoutPoint &rhs) {
        return lhs.row == rhs.row && lhs.col == rhs.col;
    }

    friend LayoutPoint operator+(const LayoutPoint &lhs, const Direction rhs) {
        switch (rhs) {
        case Direction::UP:
            return LayoutPoint{lhs.row - 1, lhs.col};
        case Direction::DOWN:
            return LayoutPoint{lhs.row + 1, lhs.col};
        case Direction::LEFT:
            return LayoutPoint{lhs.row, lhs.col - 1};
        case Direction::RIGHT:
            return LayoutPoint{lhs.row, lhs.col + 1};
        default:
            return lhs;
        }
    }
};