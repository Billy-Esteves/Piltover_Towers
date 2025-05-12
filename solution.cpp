#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*===========[ STRUCTURES ]==========*/
struct unionFind {
    vector<int> parent;

    // assign parents to the nodes
    unionFind(int n) : parent(n) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // union of different groups
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        parent[rootX] = rootY;
    }

    // find the parent of a node
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
};

struct Edge {
    int origin, destination, cost;
};

/*===========[ FUNCTIONS ]==========*/
bool comparator(const Edge& x, const Edge& y) {
    return x.cost < y.cost;
}

int canFormEdges(const vector<Edge>& edges, int maxCost, const vector<int>& mandatory, int x) {
    // assign a parent to x
    unionFind u(x);

    // if the cost of the edge is bigger than the max it isn't part of the solution
    for (const Edge& e : edges) {
        if (e.cost > maxCost) break;
        u.unite(e.origin, e.destination);
    }

    int representative = u.find(mandatory[0]);
    // check if the connection is possible
    for (size_t i = 1; i < mandatory.size(); i++) {
        if (u.find(mandatory[i]) != representative)
            return 0;
    }

    return 1;
}

int solution(int N, const vector<Edge>& edges, const vector<int>& mandatory) {
    vector<Edge> sortedEdges = edges;
    sort(sortedEdges.begin(), sortedEdges.end(), comparator);

    vector<int> cost;
    for ( Edge& x : sortedEdges) {
        // list is ordered so we only need to check the last element
        if (cost.empty() || cost.back() != x.cost) {
            cost.push_back(x.cost);
        }
    }

    // binary search
    int result = -1;
    auto low = cost.begin(), high = cost.end() - 1;
    while (low <= high) {
        auto mid = low + (high - low) / 2;
        int maxCost = *mid;

        if (canFormEdges(sortedEdges, maxCost, mandatory, N) == 1) {
            result = maxCost;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return result;
}

int preprocessing (const vector<Edge>& edges, const vector<int>& mandatory, int x) {
    // assign a parent to x
    unionFind u(x);

    // if the cost of the edge is bigger than the max it isn't part of the solution
    for (const Edge& e : edges) {
        u.unite(e.origin, e.destination);
    }

    int representative = u.find(mandatory[0]);
    // check if the connection is possible
    for (size_t i = 1; i < mandatory.size(); i++) {
        if (u.find(mandatory[i]) != representative)
            return 0;
    }

    return 1;
}

/*===========[ MAIN ]==========*/
int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int N, E;

    while (cin >> N >> E) {
        vector<Edge> edges;

        for (int i = 0; i < E; i++) {
            int origin, destination, cost;
            cin >> origin >> destination >> cost;
            edges.push_back({origin, destination, cost});
        }

        int M;
        cin >> M;
        vector<int> mandatory(M);
        for (int i = 0; i < M; i++) {
            cin >> mandatory[i];
        }

        if (preprocessing(edges, mandatory, N) == 0) {
            cout << "Impossible to connect!" << endl;
            continue;
        }

        int result = solution(N, edges, mandatory);

        if (result == -1) {
            cout << "Impossible to connect!" << endl;
        } else {
            cout << result << endl;
        }
    }

    return 0;
}