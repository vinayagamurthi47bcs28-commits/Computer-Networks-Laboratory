#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INF 1e9

// Structure to represent an edge in the graph
typedef struct {
    int to;
    int time;
} Edge;

// Structure for Min-Heap nodes
typedef struct {
    int node;
    int dist;
} HeapNode;

// Global structures for Priority Queue
HeapNode heap[10005];
int heapSize = 0;

void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void push(int node, int dist) {
    heap[heapSize].node = node;
    heap[heapSize].dist = dist;
    int curr = heapSize;
    heapSize++;
    
    while (curr > 0) {
        int parent = (curr - 1) / 2;
        if (heap[parent].dist <= heap[curr].dist) break;
        swap(&heap[parent], &heap[curr]);
        curr = parent;
    }
}

HeapNode pop() {
    HeapNode minNode = heap[0];
    heapSize--;
    heap[0] = heap[heapSize];
    
    int curr = 0;
    while (2 * curr + 1 < heapSize) {
        int left = 2 * curr + 1;
        int right = 2 * curr + 2;
        int smallest = left;
        
        if (right < heapSize && heap[right].dist < heap[left].dist) {
            smallest = right;
        }
        if (heap[curr].dist <= heap[smallest].dist) break;
        swap(&heap[curr], &heap[smallest]);
        curr = smallest;
    }
    return minNode;
}

int networkDelayTime(int** times, int timesSize, int* timesColSize, int n, int k) {
    heapSize = 0; // Reset heap size for each call
    
    // Step 1: Count out-degrees to allocate precise space for each node's adjacent edges
    int* degree = (int*)calloc(n + 1, sizeof(int));
    for (int i = 0; i < timesSize; i++) {
        degree[times[i][0]]++;
    }
    
    // Allocate the adjacency list
    Edge** graph = (Edge**)malloc((n + 1) * sizeof(Edge*));
    for (int i = 1; i <= n; i++) {
        graph[i] = (Edge*)malloc(degree[i] * sizeof(Edge));
    }
    
    // Fill the adjacency list
    int* current_offset = (int*)calloc(n + 1, sizeof(int));
    for (int i = 0; i < timesSize; i++) {
        int u = times[i][0];
        int v = times[i][1];
        int w = times[i][2];
        int idx = current_offset[u]++;
        graph[u][idx].to = v;
        graph[u][idx].time = w;
    }
    
    // Step 2: Dijkstra's algorithm setups
    int* dist = (int*)malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++) dist[i] = INF;
    
    dist[k] = 0;
    push(k, 0);
    
    // Step 3: Run the priority-queue-driven exploration
    while (heapSize > 0) {
        HeapNode curr = pop();
        int u = curr.node;
        int d = curr.dist;
        
        if (d > dist[u]) continue; // Skip obsolete states
        
        for (int i = 0; i < degree[u]; i++) {
            int v = graph[u][i].to;
            int weight = graph[u][i].time;
            
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                push(v, dist[v]);
            }
        }
    }
    
    // Step 4: Aggregate the max travel delay time
    int max_delay = 0;
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INF) {
            // Memory Cleanup before returning unreachable status
            for (int j = 1; j <= n; j++) free(graph[j]);
            free(graph); free(degree); free(current_offset); free(dist);
            return -1; 
        }
        if (dist[i] > max_delay) {
            max_delay = dist[i];
        }
    }
    
    // Dynamic allocations cleanup
    for (int j = 1; j <= n; j++) free(graph[j]);
    free(graph); free(degree); free(current_offset); free(dist);
    
    return max_delay;
}
