int totalHammingDistance(int* nums, int numsSize) {
    int total_distance = 0;
    
    for (int bit = 0; bit < 32; bit++) {
        int count_ones = 0;
        
        for (int i = 0; i < numsSize; i++) {
            if ((nums[i] >> bit) & 1) {
                count_ones++;
            }
        }
        
        int count_zeros = numsSize - count_ones;
        total_distance += count_ones * count_zeros;
    }
    
    return total_distance;
}
