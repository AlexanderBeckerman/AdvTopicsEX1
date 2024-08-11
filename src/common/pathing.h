#include <optional>
#include <queue>
#include <stack>
#include <unordered_set>

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