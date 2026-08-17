#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int main() {
    char inputString[100];
    int data[800]; // 100 characters max * 8 bits = 800 bits max
    int hamming[1000], received[1000];
    int m = 0, r = 0, totalBits;
    int i, j, parity, errorPos = 0;

    printf("HAMMING CODE - STRING TO BINARY & SINGLE BIT ERROR CORRECTION\n");

    printf("\nEnter a sentence: ");
    if (scanf("%99[^\n]", inputString) != 1) {
        printf("Invalid input string.\n");
        return 1;
    }

    // Step 1: Convert characters to 8-bit binary data bits
    int stringLength = strlen(inputString);
    int bitIndex = 1;
    for (i = 0; i < stringLength; i++) {
        char ch = inputString[i];
        for (j = 7; j >= 0; j--) {
            data[bitIndex++] = (ch >> j) & 1;
        }
    }
    m = bitIndex - 1;

    printf("Converted Binary Data (%d bits total):\n", m);
    for (i = 1; i <= m; i++) {
        printf("%d", data[i]);
        if (i % 8 == 0) printf(" ");
    }
    printf("\n");

    // Step 2: Calculate number of parity bits required
    while (pow(2, r) < (m + r + 1)) {
        r++;
    }
    totalBits = m + r;
    printf("\nNumber of parity bits = %d", r);
    printf("\nTotal bits (Data + Parity) = %d\n", totalBits);

    // Step 3: Place data bits around parity positions (Powers of 2)
    j = 1;
    for (i = 1; i <= totalBits; i++) {
        if ((i & (i - 1)) == 0) {
            hamming[i] = 0;
        } else {
            hamming[i] = data[j++];
        }
    }

    // Step 4: Calculate Parity bits (FIX: Skip the parity position itself)
    for (i = 0; i < r; i++) {
        int pos = (int)pow(2, i);
        parity = 0;
        for (j = 1; j <= totalBits; j++) {
            if (j != pos && (j & pos)) { // Check everything except itself
                parity ^= hamming[j];
            }
        }
        hamming[pos] = parity;
    }

    printf("\nGenerated Hamming Code:\n");
    for (i = 1; i <= totalBits; i++) {
        printf("%d", hamming[i]);
    }
    printf("\n");

    // Step 5: Simulate data transmission error
    for (i = 1; i <= totalBits; i++) {
        received[i] = hamming[i];
    }
    srand((unsigned int)time(NULL));
    int randomBit = rand() % totalBits + 1;
    received[randomBit] ^= 1;

    printf("\n--- TRANSMISSION ---");
    printf("\n[ALERT] Bit at position %d was flipped dynamically during transmission.\n", randomBit);
    printf("Received Code Packet:\n");
    for (i = 1; i <= totalBits; i++) {
        printf("%d", received[i]);
    }
    printf("\n");

    // Step 6: Calculate Syndrome value (FIX: Check parity including its position bit)
    errorPos = 0;
    for (i = 0; i < r; i++) {
        int pos = (int)pow(2, i);
        parity = 0;
        for (j = 1; j <= totalBits; j++) {
            if (j & pos) { // Standard check includes the parity position bit here
                parity ^= received[j];
            }
        }
        if (parity) {
            errorPos += pos;
        }
    }

    printf("\nCalculated Syndrome Value = %d", errorPos);

    // Step 7: Evaluate and Correct Error
    if (errorPos == 0 || errorPos > totalBits) {
        printf("\nNo Error Detected (or outside boundary).\n");
    } else {
        printf("\nError detected at bit position %d. Autofixing stream...", errorPos);
        received[errorPos] ^= 1;
        printf("\nCorrected Hamming Code Matrix:\n");
        for (i = 1; i <= totalBits; i++) {
            printf("%d", received[i]);
        }
        printf("\n");
    }

    // Step 8: Recover data bits and parse back to ASCII characters
    int recoveredData[800];
    int recIdx = 1;
    for (i = 1; i <= totalBits; i++) {
        if ((i & (i - 1)) != 0) {
            recoveredData[recIdx++] = received[i];
        }
    }

    printf("\nRecovered String Output: ");
    for (i = 0; i < stringLength; i++) {
        char ch = 0;
        for (j = 0; j < 8; j++) {
            ch = (ch << 1) | recoveredData[i * 8 + j + 1];
        }
        printf("%c", ch);
    }
    printf("\n");

    return 0;
}
