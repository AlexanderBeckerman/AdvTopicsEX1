#include "graph.h"

bool Graph::bfs(int start, int goal, std::vector<int> &path) {
    std::unordered_map<int, bool> visited;
    std::unordered_map<int, int> parent;
    std::queue<int> q;

    q.push(start);
    visited[start] = true;
    parent[start] = -1;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        if (node == goal) {
            int current = goal;
            while (current != -1) {
                path.push_back(current);
                current = parent[current];
            }
            std::reverse(path.begin(), path.end());
            return true;
        }

        for (auto neighbor : adjList[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
                parent[neighbor] = node;
            }
        }
    }
    return false;
}

bool Graph::dfs(int start, int goal, std::vector<int> &path) {
    std::unordered_map<int, bool> visited;
    std::unordered_map<int, int> parent;
    std::stack<int> s;

    s.push(start);
    visited[start] = true;
    parent[start] = -1;

    while (!s.empty()) {
        int node = s.top();
        s.pop();

        if (node == goal) {
            int current = goal;
            while (current != -1) {
                path.push_back(current);
                current = parent[current];
            }
            std::reverse(path.begin(), path.end());
            return true;
        }

        for (auto neighbor : adjList[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                s.push(neighbor);
                parent[neighbor] = node;
            }
        }
    }
    return false;
}