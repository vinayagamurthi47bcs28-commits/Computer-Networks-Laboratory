#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure to associate a stop with a specific bus route
typedef struct {
    int stop;
    int bus_id;
} StopBusPair;

// Comparison function used by qsort to sort pairs by stop ID
int compare_pairs(const void* a, const void* b) {
    return ((StopBusPair*)a)->stop - ((StopBusPair*)b)->stop;
}

int numBusesToDestination(int** routes, int routesSize, int* routesColSize, int source, int target) {
    // Edge case: if you are already at your destination, no buses are needed
    if (source == target) {
        return 0;
    }

    // Step 1: Count total stops across all routes to allocate a flat mapping array
    int total_elements = 0;
    for (int i = 0; i < routesSize; i++) {
        total_elements += routesColSize[i];
    }

    StopBusPair* pairs = (StopBusPair*)malloc(total_elements * sizeof(StopBusPair));
    int idx = 0;
    for (int i = 0; i < routesSize; i++) {
        for (int j = 0; j < routesColSize[i]; j++) {
            pairs[idx].stop = routes[i][j];
            pairs[idx].bus_id = i;
            idx++;
        }
    }

    // Sort the pairs by stop ID to allow binary searching / contiguous scanning
    qsort(pairs, total_elements, sizeof(StopBusPair), compare_pairs);

    // Create an index map tracking where each unique stop's contigous block starts and ends
    // Using a binary-search lookup format based on the sorted pairs array
    int* stop_start_idx = (int*)malloc(total_elements * sizeof(int));
    int* stop_end_idx = (int*)malloc(total_elements * sizeof(int));
    
    // Arrays tracking visitation to prevent cycles
    bool* visited_buses = (bool*)calloc(routesSize, sizeof(bool));
    
    // Simple, efficient closed-address hashing array for visited stops to prevent TLE
    // Max hash table size set to double the size of total elements for minimum collisions
    int hash_size = total_elements * 2;
    int* visited_stops_hash = (int*)malloc(hash_size * sizeof(int));
    for (int i = 0; i < hash_size; i++) visited_stops_hash[i] = -1;

    // Helper macro/function for hash insertion and tracking
    void mark_stop_visited(int stop) {
        int h = abs(stop) % hash_size;
        while (visited_stops_hash[h] != -1) {
            h = (h + 1) % hash_size;
        }
        visited_stops_hash[h] = stop;
    }

    bool is_stop_visited(int stop) {
        int h = abs(stop) % hash_size;
        while (visited_stops_hash[h] != -1) {
            if (visited_stops_hash[h] == stop) return true;
            h = (h + 1) % hash_size;
        }
        return false;
    }

    // Helper function to find the range of elements matching a given stop in our sorted pairs array
    void find_stop_range(int stop, int* start, int* end) {
        int low = 0, high = total_elements - 1;
        int found_idx = -1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (pairs[mid].stop == stop) {
                found_idx = mid;
                break;
            } else if (pairs[mid].stop < stop) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        if (found_idx == -1) {
            *start = -1;
            *end = -1;
            return;
        }
        int s = found_idx;
        while (s > 0 && pairs[s - 1].stop == stop) s--;
        int e = found_idx;
        while (e < total_elements - 1 && pairs[e + 1].stop == stop) e++;
        *start = s;
        *end = e;
    }

    // Check if target even exists in the entire transportation network
    int dummy_s, dummy_e;
    find_stop_range(target, &dummy_s, &dummy_e);
    if (dummy_s == -1) {
        // Free allocated memory before returning
        free(pairs); free(stop_start_idx); free(stop_end_idx);
        free(visited_buses); free(visited_stops_hash);
        return -1;
    }

    // Step 2: Initialize BFS Queue
    // We queue stops. Max size can safely be total elements.
    int* queue = (int*)malloc(total_elements * sizeof(int));
    int head = 0, tail = 0;

    queue[tail++] = source;
    mark_stop_visited(source);
    int bus_count = 0;

    // Step 3: Run Level-by-Level BFS 
    while (head < tail) {
        int level_size = tail - head;
        bus_count++;

        for (int i = 0; i < level_size; i++) {
            int curr_stop = queue[head++];

            int start_r, end_r;
            find_stop_range(curr_stop, &start_r, &end_r);
            if (start_r == -1) continue;

            // Loop through all bus lines that visit the current stop
            for (int k = start_r; k <= end_r; k++) {
                int b_id = pairs[k].bus_id;
                if (visited_buses[b_id]) continue;
                visited_buses[b_id] = true;

                // Loop through all stops available on this specific bus line
                for (int m = 0; m < routesColSize[b_id]; m++) {
                    int next_stop = routes[b_id][m];

                    if (next_stop == target) {
                        // Target found, clean up memory and return result
                        free(pairs); free(stop_start_idx); free(stop_end_idx);
                        free(visited_buses); free(visited_stops_hash); free(queue);
                        return bus_count;
                    }

                    if (!is_stop_visited(next_stop)) {
                        mark_stop_visited(next_stop);
                        queue[tail++] = next_stop;
                    }
                }
            }
        }
    }

    // Clean up memory if destination is unreachable
    free(pairs); free(stop_start_idx); free(stop_end_idx);
    free(visited_buses); free(visited_stops_hash); free(queue);
    return -1;
}
