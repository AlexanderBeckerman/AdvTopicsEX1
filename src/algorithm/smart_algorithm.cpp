#include "smart_algorithm.h"
#include "utils.h"
#include <queue>

std::stack<Direction> shortestPathToOrigin(
    const std::unordered_set<RelativePoint, RelativePointKeyHash>
        &visitedPoints,
    const RelativePoint &currentLocation);

std::stack<Direction>
shortestPath(const std::unordered_set<RelativePoint, RelativePointKeyHash>
                 &visitedPoints,
             const RelativePoint &currentLocation,
             const RelativePoint &destination);

Step SmartAlgorithm::nextStep() {
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
        } else {
            // Battery is full, we can start exploring.
            if (last_return_point.has_value()) {
                auto path_to_last_return = shortestPath(
                    visited, robot_location, *this->last_return_point);
                if (path_to_last_return.size() < steps_left / 2) {
                    predetermined_path =
                        std::make_optional(path_to_last_return);
                } else {
                    this->direction_stack = std::stack<Direction>();
                }
                last_return_point = std::nullopt;
            }
        }
    }

    // If we have a predetermined path, we should follow it.
    if (this->predetermined_path.has_value() &&
        !this->predetermined_path->empty() &&
        predetermined_path->size() < steps_left) {
        LOG(INFO) << "Following predetermined path." << std::endl;
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
        startReturn();
        auto &dir = predetermined_path->top();
        predetermined_path->pop();
        LOG(INFO) << "Returning to charging station." << dir << std::endl;
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
            LOG(INFO) << "No valid moves, and not at charging station."
                      << std::endl;
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

std::stack<Direction>
shortestPath(const std::unordered_set<RelativePoint, RelativePointKeyHash>
                 &visitedPoints,
             const RelativePoint &currentLocation,
             const RelativePoint &destination) {
    std::queue<RelativePoint> q;
    std::unordered_set<RelativePoint, RelativePointKeyHash> visited;
    std::unordered_map<RelativePoint, RelativePoint, RelativePointKeyHash>
        parent;

    // Initialize BFS from the current location.
    q.push(currentLocation);
    visited.insert(currentLocation);
    parent[currentLocation] = currentLocation;

    // Perform BFS
    while (!q.empty()) {
        RelativePoint current = q.front();
        q.pop();

        if (current == destination) {
            break; // Found shortest path to origin
        }

        // Explore neighbors
        for (auto &dir : allDirections) {
            RelativePoint next = current + dir;

            if (visited.find(next) == visited.end() &&
                visitedPoints.find(next) != visitedPoints.end()) {
                visited.insert(next);
                q.push(next);
                parent[next] = current;
            }
        }
    }

    // Reconstruct path using parent map
    std::stack<Direction> path;
    RelativePoint current = destination;

    while (current != parent[current]) {
        int dx = current.x - parent[current].x;
        int dy = current.y - parent[current].y;

        if (dx == -1 && dy == 0) {
            path.push(Direction::West);
        } else if (dx == 1 && dy == 0) {
            path.push(Direction::East);
        } else if (dx == 0 && dy == -1) {
            path.push(Direction::South);
        } else if (dx == 0 && dy == 1) {
            path.push(Direction::North);
        }

        current = parent[current];
    }

    return path;
}

std::stack<Direction> shortestPathToOrigin(
    const std::unordered_set<RelativePoint, RelativePointKeyHash>
        &visitedPoints,
    const RelativePoint &currentLocation) {
    return shortestPath(visitedPoints, currentLocation, RelativePoint{0, 0});
}

void SmartAlgorithm::startReturn() {
    last_return_point = std::make_optional<RelativePoint>(robot_location);
    LOG(INFO) << "Starting return to charging station." << std::endl;
    predetermined_path =
        std::make_optional(shortestPathToOrigin(cleaned, robot_location));
}

Step SmartAlgorithm::moveDirection(const Direction &dir) {
    robot_location = robot_location + dir;
    steps_left--;
    return directionToStep(dir);
}