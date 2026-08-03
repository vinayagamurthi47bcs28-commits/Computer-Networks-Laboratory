#include <stdbool.h>

bool hasAlternatingBits(int n) {
    // Shift n to the right by 1 to align opposite bits
    unsigned int x = n ^ (n >> 1);
    
    // Check if x is of the form 111...111 (all bits set to 1)
    return (x & (x + 1)) == 0;
}
