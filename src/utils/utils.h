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

inline std::string getOutputMessage(int exit_cond) {
    if (exit_cond == 0) {
        return "Success! no dirt left and robot is at the docking station.\n";
    } else if (exit_cond == 1) {
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

inline size_t safeSubtractOne(size_t value) {
    if (value == 0) {
        throw std::underflow_error("Value is already 0");
    }
    return value - 1;
}