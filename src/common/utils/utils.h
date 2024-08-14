#pragma once
#include "logger.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

enum Direction {
    North,
    South,
    West,
    East,
};

const std::vector<Direction> allDirections = {
    Direction::North, Direction::South, Direction::West, Direction::East};

enum class Step { North, East, South, West, Stay, Finish };

inline Step directionToStep(Direction d) {
    switch (d) {
    case Direction::North:
        return Step::North;
    case Direction::South:
        return Step::South;
    case Direction::West:
        return Step::West;
    case Direction::East:
        return Step::East;
    default:
        return Step::Finish;
    }
}

inline bool trueWithProb(int prob) { return rand() % 100 < prob; }

inline Direction oppositeDirection(const Direction &d) {
    switch (d) {
    case Direction::North:
        return Direction::South;
    case Direction::South:
        return Direction::North;
    case Direction::West:
        return Direction::East;
    case Direction::East:
        return Direction::West;
    default:
        return Direction::North; // Should not reach here (have to add default
                                 // to avoid error)
    }
}

inline Direction stepToDirection(const Step step) {
    switch (step) {
    case Step::North:
        return Direction::North;
    case Step::East:
        return Direction::East;
    case Step::South:
        return Direction::South;
    case Step::West:
        return Direction::West;
    default:
        return Direction::North; // Should not reach here (have to add default
                                 // to avoid error)
    }
}

inline std::ostream &operator<<(std::ostream &out, const Direction d) {
    switch (d) {
    case Direction::North:
        out << "UP";
        break;
    case Direction::South:
        out << "DOWN";
        break;
    case Direction::West:
        out << "LEFT";
        break;
    case Direction::East:
        out << "RIGHT";
        break;
    }
    return out;
}

inline std::string getStatus(int exit_cond) {
    if (exit_cond == 0) {
        return "FINISHED";
    } else if (exit_cond == 1) {
        return "DEAD";
    }
    return "WORKING";
}

inline std::string addPrefixToFileName(const std::string &filePath) {
    std::filesystem::path inputPath(filePath);

    // Check if the path is a regular file
    std::filesystem::path outputPath =
        inputPath.parent_path() / ("output_" + inputPath.filename().string());
    return outputPath.string();
}

inline size_t safeSubtractOne(size_t value) {
    if (value == 0) {
        throw std::underflow_error("Value is already 0");
    }
    return value - 1;
}