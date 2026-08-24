#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure to hold an edge's neighbor and its orientation flag
typedef struct {
    int to;
    int is_original; // 1 if it is an original edge (needs reversal if moving outward), 0 if reverse edge
} Edge;

int minReorder(int n, int** connections, int connectionsSize, int* connectionsColSize) {
    // Step 1: Count the degree (number of neighbors) for each node to build a flat adjacency layout
    int* degree = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < connectionsSize; i++) {
        degree[connections[i][0]]++;
        degree[connections[i][1]]++;
    }

    // Allocate memory for our adjacency list structure using head pointers into a flat array
    Edge** graph = (Edge**)malloc(n * sizeof(Edge*));
    for (int i = 0; i < n; i++) {
        graph[i] = (Edge*)malloc(degree[i] * sizeof(Edge));
    }

    // Fill the adjacency structures (reset degree array temporarily to use as write offsets)
    int* current_offset = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < connectionsSize; i++) {
        int u = connections[i][0];
        int v = connections[i][1];

        // Original outward edge u -> v (Moving from u to v goes away from 0, so cost = 1)
        int idx_u = current_offset[u]++;
        graph[u][idx_u].to = v;
        graph[u][idx_u].is_original = 1;

        // Artificial inward edge v -> u (Moving from v to u goes toward 0, so cost = 0)
        int idx_v = current_offset[v]++;
        graph[v][idx_v].to = u;
        graph[v][idx_v].is_original = 0;
    }

    // Step 2: Set up traditional BFS structures starting from the root node (0)
    int* queue = (int*)malloc(n * sizeof(int));
    bool* visited = (bool*)calloc(n, sizeof(bool));
    int head = 0, tail = 0;
    int reorder_count = 0;

    // Board node 0
    queue[tail++] = 0;
    visited[0] = true;

    // Step 3: Run the traversal
    while (head < tail) {
        int curr = queue[head++];

        // Inspect all neighbors linked to the active node
        for (int i = 0; i < degree[curr]; i++) {
            int neighbor = graph[curr][i].to;

            if (!visited[neighbor]) {
                visited[neighbor] = true;
                
                // If the edge was natively pointing FROM our current position TO the neighbor,
                // it is pointing away from 0. We must turn it around!
                if (graph[curr][i].is_original == 1) {
                    reorder_count++;
                }

                queue[tail++] = neighbor;
            }
        }
    }

    // Step 4: Clean up dynamically allocated memory
    for (int i = 0; i < n; i++) {
        free(graph[i]);
    }
    free(graph);
    free(degree);
    free(current_offset);
    free(queue);
    free(visited);

    return reorder_count;
}
