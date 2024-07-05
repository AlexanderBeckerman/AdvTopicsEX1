#pragma once
#include "layout_point.h"

struct StepInfo {
    LayoutPoint point;
    size_t battery_level;

    // TODO(Sasha): this should not be a string.
    std::string Serialize() const {
        return std::to_string(point.row) + " " + std::to_string(point.col) +
               " " + std::to_string(battery_level);
    }

    std::string toOutputString() const {
        return std::to_string(point.row) + " " + std::to_string(point.col);
    }
};

inline std::string serializeVecSteps(const std::vector<StepInfo> &steps) {
    std::string output = "";
    for (auto &step : steps) {
        output += step.Serialize() + "\n";
    }
    return output;
}