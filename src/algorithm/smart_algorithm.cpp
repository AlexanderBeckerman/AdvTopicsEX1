#include "smart_algorithm.h"
#include "utils.h"
#include <queue>

Step SmartAlgorithm::nextStep() {
    // If we are at the charging station, and battery not full we should charge.
    if (robot_location == RelativePoint{0, 0}) {
        if (battery_sensor->getBatteryState() < 100) {
            return Step::Stay;
        } else {
            // Battery is full, we can start exploring.
            return_path = std::nullopt;
        }
    }

    // If we have a return path, we should follow it.
    if (this->return_path.has_value()) {
        auto &dir = return_path->top();
        return_path->pop();
        return directionToStep(dir);
    }

    // If we are out of battery, we should return to the charging station.
    // Initilize the return path, and follow it.
    if (battery_sensor->getBatteryState() <= 1) { // TODO
        startReturn();
        auto &dir = return_path->top();
        return_path->pop();
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
            return directionToStep(dir);
        }
    }

    if (direction_stack.empty()) {
        if (robot_location == RelativePoint{0, 0}) {
            return Step::Finish;
        }
        LOG(ERROR) << "No valid moves, and not at charging station."
                   << std::endl;
    }

    // We are stuck, backtrack.
    auto &dir = direction_stack.top();
    direction_stack.pop();
    return directionToStep(oppositeDirection(dir));
}

std::stack<Step> shortestPathToOrigin(
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
    std::stack<Step> path;
    RelativePoint current = origin;

    while (current != parent[current]) {
        int dx = current.x - parent[current].x;
        int dy = current.y - parent[current].y;

        if (dx == -1 && dy == 0) {
            path.push(Step::North);
        } else if (dx == 1 && dy == 0) {
            path.push(Step::South);
        } else if (dx == 0 && dy == -1) {
            path.push(Step::West);
        } else if (dx == 0 && dy == 1) {
            path.push(Step::East);
        }

        current = parent[current];
    }

    return path;
}

void SmartAlgorithm::startReturn() {
    std::cout << "Starting return to charging station." << std::endl;
}