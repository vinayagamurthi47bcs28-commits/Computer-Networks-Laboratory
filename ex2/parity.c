#include <stdio.h>
#include <string.h>

char inputText[100], data[800], codeword[900], received[900];
char outputText[100];

int isAlphanumeric(char *str)
{
    int i;

    for (i = 0; str[i] != '\0'; i++)
    {
        char ch = str[i];

        if (!((ch >= 'a' && ch <= 'z') ||
              (ch >= 'A' && ch <= 'Z') ||
              (ch >= '0' && ch <= '9')))
        {
            return 0;
        }
    }

    return 1;
}

void stringToBinary(char *text, char *binaryResult)
{
    int i, j;

    binaryResult[0] = '\0';

    for (i = 0; text[i] != '\0'; i++)
    {
        char ch = text[i];

        for (j = 7; j >= 0; j--)
        {
            if ((ch >> j) & 1)
                strcat(binaryResult, "1");
            else
                strcat(binaryResult, "0");
        }
    }
}

void binaryToString(char *binary, char *textResult)
{
    int len = strlen(binary);
    int textIndex = 0;
    int i, j;

    for (i = 0; i + 7 < len; i += 8)
    {
        char ch = 0;

        for (j = 0; j < 8; j++)
        {
            ch = (ch << 1) | (binary[i + j] - '0');
        }

        textResult[textIndex++] = ch;
    }

    textResult[textIndex] = '\0';
}

int countOnes(char *str)
{
    int count = 0;
    int i;

    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '1')
            count++;
    }

    return count;
}

void injectErrors(int numErrors, int totalLen)
{
    int pos, i;

    for (i = 0; i < numErrors; i++)
    {
        printf("Enter bit index to flip (%d of %d) [0 to %d]: ",
               i + 1, numErrors, totalLen - 1);

        scanf("%d", &pos);

        if (pos >= 0 && pos < totalLen)
        {
            received[pos] =
                (received[pos] == '0') ? '1' : '0';
        }
    }
}

int main()
{
    char parityBit;
    int parityChoice;
    int simChoice;
    int numErrors;
    int dataLen;
    int isValid;

    printf("=== PARITY CHECK ===\n");

    printf("\n1. Even Parity\n");
    printf("2. Odd Parity\n");
    printf("Select: ");
    scanf("%d", &parityChoice);

    if (parityChoice != 1 && parityChoice != 2)
    {
        printf("Invalid choice.\n");
        return 0;
    }

    printf("\nEnter alphanumeric text: ");
    scanf("%s", inputText);

    if (!isAlphanumeric(inputText))
    {
        printf("Invalid input.\n");
        return 0;
    }

    /* Convert text to binary */
    stringToBinary(inputText, data);

    dataLen = strlen(data);

    /* Calculate parity bit */
    if (parityChoice == 1)
    {
        /* Even parity */
        parityBit = (countOnes(data) % 2 == 0) ? '0' : '1';
    }
    else
    {
        /* Odd parity */
        parityBit = (countOnes(data) % 2 != 0) ? '0' : '1';
    }

    /* Create codeword */
    strcpy(codeword, data);

    codeword[dataLen] = parityBit;
    codeword[dataLen + 1] = '\0';

    printf("\nTransmitted Codeword: %s\n", codeword);

    /* Error simulation */
    printf("\n1. Error-Free\n");
    printf("2. Introduce Errors\n");
    printf("Select: ");
    scanf("%d", &simChoice);

    strcpy(received, codeword);

    if (simChoice == 2)
    {
        printf("How many bits to flip? ");
        scanf("%d", &numErrors);

        injectErrors(numErrors, strlen(codeword));
    }

    /* Extract original data */
    strncpy(data, received, dataLen);
    data[dataLen] = '\0';

    binaryToString(data, outputText);

    printf("\nTransmitted Codeword : %s\n", codeword);
    printf("Received Codeword    : %s\n", received);
    printf("Received Message     : %s\n", outputText);

    /* Check parity */
    if (parityChoice == 1)
        isValid = (countOnes(received) % 2 == 0);
    else
        isValid = (countOnes(received) % 2 != 0);

    if (isValid)
        printf("Status               : No error detected!\n");
    else
        printf("Status               : Error detected!\n");

    return 0;
}
