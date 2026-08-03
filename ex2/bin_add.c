#include <string.h>
#include <stdlib.h>

char* addBinary(char* a, char* b) {
    int i = strlen(a) - 1;
    int j = strlen(b) - 1;
    
    // Max length of result string is max(len_a, len_b) + 1 (for carry) + 1 (for '\0')
    int max_len = (i > j ? i : j) + 2; 
    char* result = (char*)malloc((max_len + 1) * sizeof(char));
    
    result[max_len] = '\0'; // Set the null terminator at the very end
    int k = max_len - 1;    // Write index for result
    int carry = 0;
    
    // Loop from right to left until both strings and carry are exhausted
    while (i >= 0 || j >= 0 || carry > 0) {
        int sum = carry;
        
        if (i >= 0) {
            sum += a[i] - '0';
            i--;
        }
        if (j >= 0) {
            sum += b[j] - '0';
            j--;
        }
        
        result[k] = (sum % 2) + '0'; // Calculate binary digit
        carry = sum / 2;             // Calculate new carry
        k--;
    }
    
    // If the extra carry slot at the front wasn't used, return the shifted pointer
    if (k == 0) {
        return &result[1];
    }
    
    return result;
}
