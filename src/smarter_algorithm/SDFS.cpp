#include "SDFS.h"
#include "../common/AlgorithmRegistration.h"
#include "../common/pathing.h"
#include "../common/utils/utils.h"
#include <queue>

REGISTER_ALGORITHM(B_209639780_207011180);

Step B_209639780_207011180::nextStep() {
    if (steps_left == 0) {
        return Step::Finish;
    }

    if (robot_location == RelativePoint{0, 0}) {
        // No point in moving.
        if (steps_left <= 1) {
            return Step::Finish;
        }

        // If we are at the charging station, and battery not full we should
        // charge.
        if (battery_sensor->getBatteryState() < this->max_battery) {
            steps_left--;
            return Step::Stay;
        }

        // If no dirty spots around, go to an interest point
        bool clean_surroundings =
            std::none_of(allDirections.begin(), allDirections.end(),
                         [&](const auto &dir) { return isValidMove(dir); });
        if (clean_surroundings) {
            if (points_of_interest.empty()) {
                return Step::Finish;
            }
            auto &dest = points_of_interest.front();
            auto path_to_dest = shortestPath(visited, robot_location, dest);
            predetermined_path = std::make_optional(path_to_dest);
            this->direction_stack = std::stack<Direction>();
        }
    }

    // If we have a predetermined path, we should follow it.
    if (this->predetermined_path.has_value() &&
        !this->predetermined_path->empty() &&
        predetermined_path->size() < steps_left) {
        // This should never happen.
        if (predetermined_path->empty()) {
            LOG(ERROR) << "predetermined path path is empty." << std::endl;
        }
        auto &dir = predetermined_path->top();
        predetermined_path->pop();
        return this->moveDirection(dir);
    }

    // If we are out of battery, we should return to the charging station.
    // Initilize the return path, and follow it.
    auto return_path_ = shortestPathToOrigin(
        visited, robot_location); // TODO: don't compute it every time.
    size_t return_path_size = return_path_.size();
    return_path_size += dirt_sensor->dirtLevel() > 0 ? 0 : 1;
    if (battery_sensor->getBatteryState() <= return_path_size ||
        steps_left <= return_path_size) {
        points_of_interest.push(robot_location);
        startReturn();
        auto &dir = predetermined_path->top();
        predetermined_path->pop();
        return this->moveDirection(dir);
    }

    // Always stay if there is dirt (and battery).
    if (dirt_sensor->dirtLevel() > 0) {
        steps_left--;
        return Step::Stay;
    } else {
        // Tile is clean, mark as visited.
        cleaned.insert(robot_location);
    }

    // Explore via DFS.
    for (const auto &dir : allDirections) {
        if (isValidMove(dir)) {
            direction_stack.emplace(dir);
            auto step = this->moveDirection(dir);
            visited.insert(robot_location);
            return step;
        }
    }

    if (direction_stack.empty()) {
        if (robot_location != RelativePoint{0, 0}) {
            startReturn();
            steps_left = predetermined_path->size();
            auto &dir = predetermined_path->top();
            predetermined_path->pop();
            return this->moveDirection(dir);
        }
        return Step::Finish;
    }

    // We are stuck, backtrack.
    auto dir = oppositeDirection(direction_stack.top());
    direction_stack.pop();
    return this->moveDirection(dir);
}

void B_209639780_207011180::startReturn() {
    last_return_point = std::make_optional<RelativePoint>(robot_location);
    predetermined_path =
        std::make_optional(shortestPathToOrigin(cleaned, robot_location));
}

Step B_209639780_207011180::moveDirection(const Direction &dir) {
    robot_location = robot_location + dir;
    steps_left--;
    return directionToStep(dir);
}
