#pragma once
#include "logger.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STAY,
};

inline bool trueWithProb(int prob) { return rand() % 100 < prob; }

inline Direction getOppositeDirection(const Direction d) {
    switch (d) {
    case Direction::UP:
        return Direction::DOWN;
    case Direction::DOWN:
        return Direction::UP;
    case Direction::LEFT:
        return Direction::RIGHT;
    case Direction::RIGHT:
        return Direction::LEFT;
    default:
        return Direction::STAY;
    }
}

inline std::ostream &operator<<(std::ostream &out, const Direction d) {
    switch (d) {
    case Direction::UP:
        out << "UP";
        break;
    case Direction::DOWN:
        out << "DOWN";
        break;
    case Direction::LEFT:
        out << "LEFT";
        break;
    case Direction::RIGHT:
        out << "RIGHT";
        break;
    case Direction::STAY:
        out << "STAY";
        break;
    }
    return out;
}

struct Coordinate {
    int x; // Will represent the col idx in the map.
    int y; // Will represent the row idx in the map.

    friend std::ostream &operator<<(std::ostream &os, const Coordinate &loc) {
        os << "(" << loc.x << "," << loc.y << ")";
        return os;
    }

    friend bool operator==(const Coordinate &lhs, const Coordinate &rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    friend Coordinate operator+(const Coordinate &lhs, const Direction rhs) {
        switch (rhs) {
        case Direction::UP:
            return Coordinate{lhs.x, lhs.y + 1};
        case Direction::DOWN:
            return Coordinate{lhs.x, lhs.y - 1};
        case Direction::LEFT:
            return Coordinate{lhs.x - 1, lhs.y};
        case Direction::RIGHT:
            return Coordinate{lhs.x + 1, lhs.y};
        default:
            return lhs;
        }
    }

    friend Coordinate operator+(const Coordinate &lhs, const Coordinate &rhs) {
        return Coordinate{lhs.x + rhs.x, lhs.y + rhs.y};
    }
};

struct Location : public Coordinate {
    Location(int x, int y) : Coordinate{x, y} {}

    friend Location operator+(const Location &lhs, const Direction rhs) {
        Coordinate cord = (Coordinate)lhs + rhs;
        return Location{cord.x, cord.y};
    }

    bool isChargingStation() const { return x == 0 && y == 0; }
};

// Hash function for Location struct.
struct LocationKeyHash {
    std::size_t operator()(const Coordinate &k) const {
        return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1);
    }
};

// Compare function for Location struct.
struct LocationKeyEqual {
    bool operator()(const Coordinate &lhs, const Coordinate &rhs) const {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

struct StepInfo {
    Coordinate point;
    size_t battery_level;

    // TODO(Sasha): this should not be a string.
    std::string Serialize() const {
        return std::to_string(point.y) + " " + std::to_string(point.x) + " " +
               std::to_string(battery_level);
    }

    std::string toOutputString() const {
        return std::to_string(point.y) + " " + std::to_string(point.x);
    }
};

inline std::string serializeVecSteps(const std::vector<StepInfo> &steps) {
    std::string output = "";
    for (auto &step : steps) {
        output += step.Serialize() + "\n";
    }
    return output;
}

inline std::string getOutputMessage(int exit_cond) {
    if (exit_cond == 0) {
        return "Success! no dirt left and robot is at the docking station.\n";
    }
    else if(exit_cond == 1){
        return "Battery is empty and the robot is stuck!\n";
    }
    return "Reached max steps allowed.\n";
}

inline std::string addPrefixToFileName(const std::string &filePath) {
    std::filesystem::path inputPath(filePath);

    // Check if the path is a regular file
    std::filesystem::path outputPath =
        inputPath.parent_path() / ("output_" + inputPath.filename().string());
    return outputPath.string();
}