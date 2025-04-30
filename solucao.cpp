#include <iostream>
#include <vector>
#include <unordered_set>

/*===========[ FUNCTION/GLOBAL VARS DECLARATION ]==========*/

std::vector<std::vector<std::pair<int, int>>> createGraph(int **edges, int E, int N);
void depthFirstSearch(int node, std::vector<std::vector<int>>& graph, std::vector<bool>& visited);
bool mandatoryConnected(int* mandatoryTowers, int M, std::vector<std::vector<int>>& graph);

/*===========[ FUNCTIONS ]==========*/

// Created the undirected graph from the edges. 
std::vector<std::vector<std::pair<int, int>>> createGraph(int **edges, int E, int N) {

    // List of lists of integers: each towers stores a list of its neighbors
    std::vector<std::vector<std::pair<int, int>>> newGraph(N);  // (neighbor, cost)

    for (int i = 0; i < E; ++i) {
        int u = edges[i][0];
        int v = edges[i][1];
        int cost = edges[i][2];

        newGraph[u].push_back({v, cost});
        newGraph[v].push_back({u, cost});  // since it's undirected
    }
    return newGraph;
}

// Perform a depth search and mark all reachable nodes from 'node' with true
void depthFirstSearch(int node, std::vector<std::vector<std::pair<int, int>>>& graph, std::vector<bool>& visited) {
    visited[node] = true;
    for (std::pair<int, int> neighbor : graph[node]) {
        if (!visited[neighbor.first]) {
            depthFirstSearch(neighbor.first, graph, visited);
        }
    }
}

// Check if all MANDATORY towers are connected.
bool mandatoryConnected(int* mandatoryTowers, int M, std::vector<std::vector<std::pair<int, int>>>& graph) {
    std::vector<bool> visited(graph.size(), false);

    // Start with ONE mandatory tower. If all mandatory towers were reachable, then all is good
    depthFirstSearch(mandatoryTowers[0], graph, visited);

    for (int i = 0; i < M; ++i) {
        if (!visited[mandatoryTowers[i]])
            return false;
    }
    return true;
}

/*===========[ MAIN ]==========*/

int main() {
    /*
        Input:
        line containing N towers and E edges separated by white space
        E lines follow, each representing a connection between two towers
        and its cost of that edge.
        Finnaly, a line with the numbers of mandatory towers M, and M lines
        with the mandatory towers.

        Example:
            5 6  <-- N towers, E edges
            0 1 5  <-- edge 0-1 with cost 5
            0 2 6
            1 3 8
            2 3 7
            2 4 9
            3 4 7
            3  <-- M mandatory towers
            0  <-- mandatory tower 0
            3
            4
            
        Output:
        For each test case we print wither the highest edge value required 
        to connect all mandatory towers or 'Impossible to connect!' if it
        is not possible to connect all mandatory towers.
    */

    int N, E, M;

    while(std::cin >> N >> E) {
        int **edges = new int*[E];
        for(int i = 0; i < E; i++) {
            edges[i] = new int[3];
            std::cin >> edges[i][0] >> edges[i][1] >> edges[i][2];
        }

        std::cin >> M;
        int *mandatory = new int[M];
        for(int i = 0; i < M; i++) {
            std::cin >> mandatory[i];
        }

        /*===========[ PRE PROCESSING ]==========*/

        /*
        Pre-processing:
           1. Check if the graph separates mandatory towers. If so, solution is impossible.
        */
       std::vector<std::vector<std::pair<int, int>>> graph = createGraph(edges, E, N);
        
        // If all mandatory towers are not connected, print apropriate message and carry on
        if(!mandatoryConnected(mandatory, M, graph)) {
            std::cout << "Impossible to connect!\n";

            //Free memory and continue 
            for(int i = 0; i < E; i++) {
                delete[] edges[i];
            }
            delete[] edges;
            delete[] mandatory;
            continue;
        }
        /*===========[ SOLUTION ]==========*/
        
        /* 
        // Kruskal's algorithm to find the maximum edge value in the MST
        // of the mandatory towers
        // ...

        // Print the result
        // ...
        */

        //Free memory 
        for(int i = 0; i < E; i++) {
            delete[] edges[i];
        }
        delete[] edges;
        delete[] mandatory;
    }

    return 0;
}