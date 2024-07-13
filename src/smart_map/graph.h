#pragma once
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>
#include <algorithm>

// Graph class
class Graph {
private:
    std::unordered_map<int, std::list<int>> adjList;

public:
    // Add an edge to the graph
    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u); // If the graph is undirected
    }

    // BFS to find a path between two nodes
    bool bfs(int start, int goal, std::vector<int> &path);

    // DFS to find a path between two nodes
    bool dfs(int start, int goal, std::vector<int> &path);
};
