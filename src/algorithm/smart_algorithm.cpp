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
    // If we are at the charging station, and battery not full we should charge.
    if (robot_location == RelativePoint{0, 0}) {
        if (battery_sensor->getBatteryState() <
            20) { // CHANGE!!!!!!!!!!!!!!!!!!!!!!!!
            steps_left--;
            return Step::Stay;
        } else {
            // Battery is full, we can start exploring.
            if (last_return_point.has_value()) {
                predetermined_path = shortestPath(visited, robot_location,
                                                  *this->last_return_point);
            } else {
                predetermined_path = std::nullopt;
            }
            this->direction_stack = std::stack<Direction>();
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
        robot_location = robot_location + dir;
        steps_left--;
        return directionToStep(dir);
    }

    // If we are out of battery, we should return to the charging station.
    // Initilize the return path, and follow it.
    auto return_path_ = shortestPathToOrigin(
        visited, robot_location); // TODO: don't compute it every time.
    if (battery_sensor->getBatteryState() - 1 <= return_path_.size()) {
        startReturn();
        auto &dir = predetermined_path->top();
        predetermined_path->pop();
        robot_location = robot_location + dir;
        steps_left--;
        return directionToStep(dir);
    }

    // Always stay if there is dirt (and battery).
    if (dirt_sensor->dirtLevel() > 0) {
        return Step::Stay;
    } else {
        // Tile is clean, mark as visited.
        cleaned.insert(robot_location);
    }

    // Explore via DFS.
    for (const auto &dir : allDirections) {
        if (isValidMove(dir)) {
            direction_stack.emplace(dir);
            robot_location = robot_location + dir;
            visited.insert(robot_location);
            steps_left--;
            return directionToStep(dir);
        }
    }

    if (direction_stack.empty()) {
        if (robot_location != RelativePoint{0, 0}) {
            LOG(ERROR) << "No valid moves, and not at charging station."
                       << std::endl;
        }
        steps_left--;
        return Step::Finish;
    }

    // We are stuck, backtrack.
    auto &dir = direction_stack.top();
    direction_stack.pop();
    robot_location = robot_location + oppositeDirection(dir);
    steps_left--;
    return directionToStep(oppositeDirection(dir));
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
            path.push(Direction::LEFT);
        } else if (dx == 1 && dy == 0) {
            path.push(Direction::RIGHT);
        } else if (dx == 0 && dy == -1) {
            path.push(Direction::DOWN);
        } else if (dx == 0 && dy == 1) {
            path.push(Direction::UP);
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