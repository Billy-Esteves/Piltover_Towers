#include <iostream>
#include <vector>
#include <algorithm>
#include <array>

/*===========[ FUNCTION DECLARATIONS ]==========*/

std::vector<std::vector<std::pair<int, int>>> createGraph(const std::vector<std::array<int, 3>>& edges, int N);
void depthFirstSearch(int node, const std::vector<std::vector<std::pair<int, int>>>& graph, std::vector<bool>& visited);
bool mandatoryConnected(const std::vector<int>& mandatoryTowers, const std::vector<std::vector<std::pair<int, int>>>& graph);
int solutionAlgorithm(const std::vector<std::vector<std::pair<int, int>>>& graph, int currentTower, int edgeCost,
                      std::vector<bool>& visited, std::vector<std::pair<int, bool>>& mandatoryTowers, bool& allConnected);

/*===========[ FUNCTIONS ]==========*/

std::vector<std::vector<std::pair<int, int>>> createGraph(const std::vector<std::array<int, 3>>& edges, int N) {
    std::vector<std::vector<std::pair<int, int>>> newGraph(N);  // (neighbor, cost)
    for (const auto& edge : edges) {
        int u = edge[0], v = edge[1], cost = edge[2];
        newGraph[u].emplace_back(v, cost);
        newGraph[v].emplace_back(u, cost);
    }

    for (auto& adjList : newGraph) {
        std::sort(adjList.begin(), adjList.end(), [](auto& a, auto& b) {
            return a.second < b.second;
        });
    }

    return newGraph;
}

void depthFirstSearch(int node, const std::vector<std::vector<std::pair<int, int>>>& graph, std::vector<bool>& visited) {
    visited[node] = true;
    for (auto [neighbor, _] : graph[node]) {
        if (!visited[neighbor]) {
            depthFirstSearch(neighbor, graph, visited);
        }
    }
}

bool mandatoryConnected(const std::vector<int>& mandatoryTowers, const std::vector<std::vector<std::pair<int, int>>>& graph) {
    std::vector<bool> visited(graph.size(), false);
    depthFirstSearch(mandatoryTowers[0], graph, visited);
    for (int tower : mandatoryTowers) {
        if (!visited[tower]) return false;
    }
    return true;
}

bool allMandatoryVisited(const std::vector<std::pair<int, bool>>& mandatoryTowers) {
    for (const auto& [_, visited] : mandatoryTowers)
        if (!visited) return false;
    return true;
}

int solutionAlgorithm(const std::vector<std::vector<std::pair<int, int>>>& graph, int currentTower, int edgeCost,
                      std::vector<bool>& visited, std::vector<std::pair<int, bool>>& mandatoryTowers, bool& allConnected) {
    
    visited[currentTower] = true;

    for (auto& [tower, seen] : mandatoryTowers) {
        if (tower == currentTower) {
            seen = true;
            break;
        }
    }

    if (allMandatoryVisited(mandatoryTowers)) {
        allConnected = true;
        return edgeCost;
    }

    int maxEdge = -1;

    for (const auto& [neighbor, cost] : graph[currentTower]) {
        if (!visited[neighbor]) {
            int recursiveMax = solutionAlgorithm(graph, neighbor, std::max(edgeCost, cost), visited, mandatoryTowers, allConnected);
            if (allConnected) return recursiveMax;
        }
    }

    return maxEdge;
}

/*===========[ MAIN ]==========*/

int main() {
    int N, E;

    while (std::cin >> N >> E) {
        std::vector<std::array<int, 3>> edges(E);
        for (int i = 0; i < E; ++i) {
            std::cin >> edges[i][0] >> edges[i][1] >> edges[i][2];
        }

        int M;
        std::cin >> M;
        std::vector<int> mandatory(M);
        std::vector<std::pair<int, bool>> visitedMandatory;
        for (int i = 0; i < M; ++i) {
            std::cin >> mandatory[i];
            visitedMandatory.emplace_back(mandatory[i], false);
        }

        auto graph = createGraph(edges, N);

        if (!mandatoryConnected(mandatory, graph)) {
            std::cout << "Impossible to connect!\n";
            continue;
        }

        std::vector<bool> visited(N, false);
        bool allConnected = false;
        int result = solutionAlgorithm(graph, mandatory[0], 0, visited, visitedMandatory, allConnected);

        std::cout << result << "\n";
    }

    return 0;
}
