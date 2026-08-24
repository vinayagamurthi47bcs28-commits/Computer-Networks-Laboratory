#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define INF 1e9

// Structure for Min-Heap nodes
typedef struct {
    int encoded_cell; // row * cols + col
    int effort;
} HeapNode;

HeapNode heap[10005]; // Safe allocation based on max grid dimensions (100x100)
int heapSize = 0;

void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void push(int cell, int effort) {
    heap[heapSize].encoded_cell = cell;
    heap[heapSize].effort = effort;
    int curr = heapSize;
    heapSize++;
    
    while (curr > 0) {
        int parent = (curr - 1) / 2;
        if (heap[parent].effort <= heap[curr].effort) break;
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
        
        if (right < heapSize && heap[right].effort < heap[left].effort) {
            smallest = right;
        }
        if (heap[curr].effort <= heap[smallest].effort) break;
        swap(&heap[curr], &heap[smallest]);
        curr = smallest;
    }
    return minNode;
}

int minimumEffortPath(int** heights, int heightsSize, int* heightsColSize) {
    int rows = heightsSize;
    int cols = heightsColSize[0];
    heapSize = 0; // Reset heap sizing state
    
    // Allocate a flat 2D matrix array for tracking minimum paths
    int** min_effort = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        min_effort[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            min_effort[i][j] = INF;
        }
    }
    
    // Directions vectors for standard 4-way grid traversal
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    
    // Initialize starting source node
    min_effort[0][0] = 0;
    push(0, 0); // Encode (0,0) as 0 * cols + 0 = 0
    
    while (heapSize > 0) {
        HeapNode curr = pop();
        int curr_cell = curr.encoded_cell;
        int curr_effort = curr.effort;
        
        int r = curr_cell / cols;
        int c = curr_cell % cols;
        
        // If we reached the bottom-right corner, return the answer immediately
        if (r == rows - 1 && c == cols - 1) {
            for (int i = 0; i < rows; i++) free(min_effort[i]);
            free(min_effort);
            return curr_effort;
        }
        
        // Skip obsolete paths
        if (curr_effort > min_effort[r][c]) continue;
        
        // Explore all 4 adjacent neighbors
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                // Determine step absolute cost
                int step_cost = abs(heights[r][c] - heights[nr][nc]);
                // Compute path total maximum effort bottleneck
                int new_effort = (curr_effort > step_cost) ? curr_effort : step_cost;
                
                if (new_effort < min_effort[nr][nc]) {
                    min_effort[nr][nc] = new_effort;
                    push(nr * cols + nc, new_effort);
                }
            }
        }
    }
    
    // Cleanup fallback statement
    for (int i = 0; i < rows; i++) free(min_effort[i]);
    free(min_effort);
    return 0;
}
