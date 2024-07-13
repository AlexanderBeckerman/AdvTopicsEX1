#include "smart_algorithm.h"
#include "utils.h"
#include <queue>

std::stack<Direction> shortestPathToOrigin(
    const std::unordered_set<RelativePoint, RelativePointKeyHash>
        &visitedPoints,
    RelativePoint currentLocation);

Step SmartAlgorithm::nextStep() {
    // If we are at the charging station, and battery not full we should charge.
    if (robot_location == RelativePoint{0, 0}) {
        if (battery_sensor->getBatteryState() <
            20) { // CHANGE!!!!!!!!!!!!!!!!!!!!!!!!
            return Step::Stay;
        } else {
            // Battery is full, we can start exploring.
            return_path = std::nullopt;
            this->direction_stack = std::stack<Direction>();
        }
    }

    // If we have a return path, we should follow it.
    if (this->return_path.has_value()) {
        // This should never happen.
        if (return_path->empty()) {
            LOG(ERROR) << "Return path is empty." << std::endl;
        }
        LOG(INFO) << "Following return path." << std::endl;
        auto &dir = return_path->top();
        return_path->pop();
        robot_location = robot_location + dir;
        return directionToStep(dir);
    }

    // If we are out of battery, we should return to the charging station.
    // Initilize the return path, and follow it.
    auto return_path_ = shortestPathToOrigin(visited, robot_location); // TODO
    if (battery_sensor->getBatteryState() - 1 <= return_path_.size()) {
        startReturn();
        auto &dir = return_path->top();
        return_path->pop();
        robot_location = robot_location + dir;
        return directionToStep(dir);
    }

    // Always stay if there is dirt (and battery).
    if (dirt_sensor->dirtLevel() > 0) {
        return Step::Stay;
    } else {
        // Tile is clean, mark as visited.
        visited.insert(robot_location);
    }

    // Explore via DFS.
    for (const auto &dir : allDirections) {
        if (isValidMove(dir)) {
            direction_stack.emplace(dir);
            robot_location = robot_location + dir;
            return directionToStep(dir);
        }
    }

    if (direction_stack.empty()) {
        if (robot_location != RelativePoint{0, 0}) {
            LOG(ERROR) << "No valid moves, and not at charging station."
                       << std::endl;
        }
        return Step::Finish;
    }

    // We are stuck, backtrack.
    auto &dir = direction_stack.top();
    direction_stack.pop();
    robot_location = robot_location + oppositeDirection(dir);
    return directionToStep(oppositeDirection(dir));
}

std::stack<Direction> shortestPathToOrigin(
    const std::unordered_set<RelativePoint, RelativePointKeyHash>
        &visitedPoints,
    RelativePoint currentLocation) {
    std::queue<RelativePoint> q;
    std::unordered_set<RelativePoint, RelativePointKeyHash> visited;
    std::unordered_map<RelativePoint, RelativePoint, RelativePointKeyHash>
        parent;

    // Initialize BFS from the current location.
    q.push(currentLocation);
    visited.insert(currentLocation);
    parent[currentLocation] = currentLocation;

    RelativePoint origin{0, 0};

    // Perform BFS
    while (!q.empty()) {
        RelativePoint current = q.front();
        q.pop();

        if (current == origin) {
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
    RelativePoint current = origin;

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

void SmartAlgorithm::startReturn() {
    std::cout << "Starting return to charging station." << std::endl;
    LOG(INFO) << "Starting return to charging station." << std::endl;
    return_path =
        std::make_optional(shortestPathToOrigin(visited, robot_location));
    std::cout << "Return path length: " << return_path->size() << std::endl;
}