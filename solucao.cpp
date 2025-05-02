#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

/*===========[ FUNCTION/GLOBAL VARS DECLARATION ]==========*/

bool CONTINUESEARCH = true;

vector<vector<pair<int, int>>> createGraph(int **edges, int E, int N);
void depthFirstSearch(int node, vector<vector<int>>& graph, vector<bool>& visited);
bool mandatoryConnected(int* mandatoryTowers, int M, vector<vector<int>>& graph);
int solutionAlgorythm(vector<vector<pair<int, int>>> graph, int currentTower, int edgeCost, bool* visited, vector<pair<int, bool>> mandatory);

/*===========[ FUNCTIONS ]==========*/

// Created the undirected graph from the edges.
vector<vector<pair<int, int>>> createGraph(int **edges, int E, int N) {

    // List of lists of integers: each towers stores a list of its neighbors
    vector<vector<pair<int, int>>> newGraph(N);  // (neighbor, cost)

    for (int i = 0; i < E; ++i) {
        int u = edges[i][0];
        int v = edges[i][1];
        int cost = edges[i][2];

        newGraph[u].push_back({v, cost});
        newGraph[v].push_back({u, cost});
    }

    // Sort by cost (helps with main solution)
    for (vector<pair<int, int>>& element : newGraph) {
        sort(element.begin(), element.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.second < b.second;
        });
    }

    return newGraph;
}

// Perform a depth search and mark all reachable nodes from 'node' with true
void depthFirstSearch(int node, vector<vector<pair<int, int>>>& graph, vector<bool>& visited) {
visited[node] = true;
for (pair<int, int> neighbor : graph[node]) {
if (!visited[neighbor.first]) {
depthFirstSearch(neighbor.first, graph, visited);
}
}
}

// Check if all MANDATORY towers are connected.
bool mandatoryConnected(int* mandatoryTowers, int M, vector<vector<pair<int, int>>>& graph) {
vector<bool> visited(graph.size(), false);

// Start with ONE mandatory tower. If all mandatory towers were reachable, then all is good
depthFirstSearch(mandatoryTowers[0], graph, visited);

for (int i = 0; i < M; ++i) {
if (!visited[mandatoryTowers[i]])
return false;
}
return true;
}

int solutionAlgorythm(vector<vector<pair<int, int>>> graph, int currentTower, int edgeCost, bool* visited, vector<pair<int, bool>> mandatory) {
/*
    1 - For every tower we check a list with all the towers, but also a
    special list, containing only the mandatory towers, if the current
    is one of them.

    2 - If all mandatory towers have been checked, we backtrack the path we
    have created.

    3- If it is not over yet, we iterate through all the paths available
    to the current tower, from least to most costly of course.
    We do not go to towers that have already been visited, to avoid loops.

    4 - We check the cost of the path we went down through. If the cost
    returned is greater than the cost of the path we took to get here,
    we return that value as the currentBiggestEdge. If not, then
    currentBiggestEdge is the cost of the edge that lead us to our
    current position.
*/

int currentBiggestEdge = -1;

if (CONTINUESEARCH) {
// 1 - Mark the tower as visited
visited[currentTower] = true;

// 1 - Mark in the mandatory list, if its a mandatory tower
for(pair<int, bool>& tower : mandatory) {
if (tower.first == currentTower) {
tower.second = true;
break;
}
}

// 2 - Check if all mandatory towers have been visited
CONTINUESEARCH = false;
for(pair<int, bool> tower : mandatory) {
if (!tower.second) {
CONTINUESEARCH = true;
break;
}
}
if(!CONTINUESEARCH) return edgeCost; // The cost of the last path

// 3 - Continue the search
for (pair<int, int> neighbour : graph[currentTower]) {    //<--- already sorted by cost            if(!visited[neighbour.first]) {
// 4 - Check all edges
currentBiggestEdge = solutionAlgorythm(graph, neighbour.first, neighbour.second, visited, mandatory);
if(edgeCost > currentBiggestEdge) currentBiggestEdge = edgeCost;
if(!CONTINUESEARCH) break;
}
}

return currentBiggestEdge;
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

    int N, E, M, maxEdgeValue = 0;


    while(cin >> N >> E) {

        int **edges = new int*[E];
        bool *visitedTowers = new bool[N];
        vector<pair<int, bool>> visitedMandatory;
        for(int i = 0; i < N; ++i) {
            visitedTowers[i] = false;
        }

        for(int i = 0; i < E; i++) {
            edges[i] = new int[3];
            cin >> edges[i][0] >> edges[i][1] >> edges[i][2];
        }

        cin >> M;
        int *mandatory = new int[M];
        for(int i = 0; i < M; i++) {
            cin >> mandatory[i];
            visitedMandatory.push_back({mandatory[i], false});
        }

        /*===========[ PRE PROCESSING ]==========*/

        /*
        Pre-processing:
           1. Check if the graph separates mandatory towers. If so, solution is impossible.
        */
        vector<vector<pair<int, int>>> graph = createGraph(edges, E, N);

        // If all mandatory towers are not connected, print apropriate message and carry on
        if(!mandatoryConnected(mandatory, M, graph)) {
            cout << "Impossible to connect!\n";

            //Free memory and continue
            for(int i = 0; i < E; i++) {
                delete[] edges[i];
            }
            delete[] edges;
            delete[] mandatory;
            delete[] visitedTowers;
            continue;
        }
        /*===========[ SOLUTION ]==========*/

        /*
            João's solution:
            1. Sort the neighbours of all towers in terms of cost. (Done in createGraph)
            2. Create the following variables:
                - bool visitedTowers[N]: to check all visited towers. this is used to avoid cycles.
                - bool visitedMandatory[M]: to check all visited mandatory towers. once all true, we can stop the search.
                - bool CONTINUESEARCH: to control the search for mandatory towers. once all mandatory towers are visited, turn to false
            3. create a recursive function(graph, current_tower, biggestEdgeYet, visitedTowers, mandatoryTowers):
                currentBiggestEdge = biggestEdgeYet
                tempEdge = 0

                if CONTINUESEARCH is true && !visitedTowers[current_tower]:
                    mark it as visited in visitedTowers.

                    if 'current_tower' is mandatory:
                        mark it as visited in visitedMandatory.

                    if all mandatory towers are visited:
                        CONTINUESEARCH = false;
                        return currentBiggestEdge

                    else:
                        for (neighbour : graph[current_tower],first):    <--- already sorted by cost
                            tempEdge = function(graph, neighbour, biggestEdgeYet, visitedTowers, mandatoryTowers)
                            if tempEdge > currentBiggestEdge: currentBiggestEdge = tempEdge
                else:
                    return currentBiggestEdge

        // Kruskal's algorithm to find the maximum edge value in the MST
        // of the mandatory towers
        */

        // Print the result


        maxEdgeValue = solutionAlgorythm(graph, mandatory[0], 0, visitedTowers, visitedMandatory);

        cout << maxEdgeValue << "\n";

        //Free memory 
        for(int i = 0; i < E; i++) {
            delete[] edges[i];
        }
        delete[] edges;
        delete[] mandatory;
        delete[] visitedTowers;

        // Reset variables for next test case
        maxEdgeValue = 0;
        CONTINUESEARCH = true;
    }

    return 0;
}