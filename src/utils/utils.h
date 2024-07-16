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
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STAY,
};

const std::vector<Direction> allDirections = {
    Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

enum class Step { North, East, South, West, Stay, Finish };

inline Step directionToStep(Direction d) {
    switch (d) {
    case Direction::UP:
        return Step::North;
    case Direction::DOWN:
        return Step::South;
    case Direction::LEFT:
        return Step::West;
    case Direction::RIGHT:
        return Step::East;
    case Direction::STAY:
        return Step::Stay;
    default:
        return Step::Finish;
    }
}

inline bool trueWithProb(int prob) { return rand() % 100 < prob; }

inline Direction oppositeDirection(const Direction &d) {
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

inline Direction stepToDirection(const Step step) {
    switch (step) {
    case Step::North:
        return Direction::UP;
    case Step::East:
        return Direction::RIGHT;
    case Step::South:
        return Direction::DOWN;
    case Step::West:
        return Direction::LEFT;
    case Step::Stay:
        return Direction::STAY;
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