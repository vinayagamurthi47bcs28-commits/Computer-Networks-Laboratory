#include <stdio.h>
#include <stdlib.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* maxSlidingWindow(int* nums, int numsSize, int k, int* returnSize) {
    if (numsSize == 0 || k == 0) {
        *returnSize = 0;
        return NULL;
    }

    int* result = (int*)malloc((numsSize - k + 1) * sizeof(int));
    *returnSize = numsSize - k + 1;

    // Deque to store indices of array elements
    int* deque = (int*)malloc(numsSize * sizeof(int));
    int head = 0; // Front of the deque
    int tail = 0; // Back of the deque

    for (int i = 0; i < numsSize; i++) {
        // 1. Remove indices that are out of the current sliding window bounds
        if (head < tail && deque[head] < i - k + 1) {
            head++;
        }

        // 2. Maintain monotonic property: Remove elements smaller than the current element
        while (head < tail && nums[deque[tail - 1]] <= nums[i]) {
            tail--;
        }

        // 3. Add the current element's index to the back of the deque
        deque[tail++] = i;

        // 4. The maximum of the current window is always at the front of the deque
        if (i >= k - 1) {
            result[i - k + 1] = nums[deque[head]];
        }
    }

    free(deque);
    return result;
}
