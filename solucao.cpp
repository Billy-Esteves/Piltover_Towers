#include <iostream>


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

        // Kruskal's algorithm to find the maximum edge value in the MST
        // of the mandatory towers
        // ...

        // Print the result
        // ...

        // Free memory
        for(int i = 0; i < E; i++) {
            delete[] edges[i];
        }
        delete[] edges;
        delete[] mandatory;
    }

    return 0;
}