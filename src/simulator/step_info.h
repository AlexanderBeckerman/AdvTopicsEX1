#pragma once
#include "../common/utils/layout_point.h"

struct StepInfo {
    LayoutPoint point;
    size_t battery_level;
    Step step;
    size_t steps_left;

    // TODO(Sasha): this should not be a string.
    std::string Serialize() const {
        return std::to_string(point.row) + " " + std::to_string(point.col) +
               " " + std::to_string(battery_level) + " " +
               std::to_string(steps_left);
    }

    std::string toOutputString() const {
        return std::to_string(point.row) + " " + std::to_string(point.col);
    }

    std::string toOutputStep() const {
        switch (step) {
        case Step::North:
            return "N";
        case Step::East:
            return "E";
        case Step::South:
            return "S";
        case Step::West:
            return "W";
        case Step::Stay:
            return "s";
        case Step::Finish:
            return "F";
        }
        return "Unknown";
    }
};

inline std::string serializeVecSteps(const std::vector<StepInfo> &steps) {
    std::string output = "";
    for (auto &step : steps) {
        output += step.Serialize() + "\n";
    }
    return output;
}