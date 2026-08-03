#include <stdlib.h>

// A basic hash set structure tailored for tracking unique overall OR results
typedef struct {
    int *table;
    int size;
} HashSet;

void insert(HashSet *set, int val) {
    int idx = (val & 0x7FFFFFFF) % set->size;
    while (set->table[idx] != -1) {
        if (set->table[idx] == val) return; // Value already exists
        idx = (idx + 1) % set->size;
    }
    set->table[idx] = val;
}

int count_unique(HashSet *set) {
    int count = 0;
    for (int i = 0; i < set->size; i++) {
        if (set->table[i] != -1) count++;
    }
    return count;
}

int subarrayBitwiseORs(int* arr, int arrSize) {
    if (arrSize == 0) return 0;

    // Use a large primary hash table to hold global unique bitwise results.
    // Max unique elements rarely exceed 32 * arrSize, making a 10x capacity safe.
    int hashCapacity = arrSize * 15;
    if (hashCapacity < 100) hashCapacity = 100;
    
    HashSet uniqueSet;
    uniqueSet.size = hashCapacity;
    uniqueSet.table = (int*)malloc(hashCapacity * sizeof(int));
    for (int i = 0; i < hashCapacity; i++) uniqueSet.table[i] = -1;

    // Flattening sets: 'current_ors' maintains unique ORs ending at index i.
    // Because size <= 32, we can manage it entirely inside simple stack buffers.
    int current_ors[33];
    int current_size = 0;

    for (int i = 0; i < arrSize; i++) {
        int next_ors[33];
        int next_size = 0;

        // Condition 1: Subarray starting and ending exactly at index i
        next_ors[next_size++] = arr[i];
        insert(&uniqueSet, arr[i]);

        // Condition 2: Extend all previous valid subarrays into index i
        for (int j = 0; j < current_size; j++) {
            int new_or = current_ors[j] | arr[i];
            
            // Deduplicate immediately in-place within the tiny 32-element buffer
            if (new_or != next_ors[next_size - 1]) {
                next_ors[next_size++] = new_or;
                insert(&uniqueSet, new_or);
            }
        }

        // Move the next frontier buffer forward to prepare for index i + 1
        current_size = next_size;
        for (int j = 0; j < current_size; j++) {
            current_ors[j] = next_ors[j];
        }
    }

    int result = count_unique(&uniqueSet);
    free(uniqueSet.table);
    return result;
}
