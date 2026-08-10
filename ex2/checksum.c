#include <stdio.h>
#include <string.h>

char inputText[100];
char data[100];
char codeword[100];
char received[100];
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

void calculateChecksum(char *b1, char *b2, char *result)
{
    int carry = 0;
    int i;

    /* Binary addition */
    for (i = 7; i >= 0; i--)
    {
        int sum = (b1[i] - '0') +
                  (b2[i] - '0') +
                  carry;

        result[i] = (sum % 2) + '0';
        carry = sum / 2;
    }

    /* End-around carry */
    if (carry == 1)
    {
        for (i = 7; i >= 0; i--)
        {
            int sum = (result[i] - '0') + carry;

            result[i] = (sum % 2) + '0';
            carry = sum / 2;
        }
    }

    result[8] = '\0';

    /* Complement */
    for (i = 0; i < 8; i++)
    {
        result[i] =
            (result[i] == '0') ? '1' : '0';
    }
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
    char block1[10];
    char block2[10];
    char checksum[10];

    char rBlock1[10];
    char rBlock2[10];
    char rChecksum[10];

    char receivedData[20];

    int simChoice;
    int numErrors;
    int separateChoice;

    printf("=== CHECKSUM METHOD ===\n");

    printf("\nEnter exactly 2 alphanumeric characters: ");
    scanf("%s", inputText);

    if (strlen(inputText) != 2 ||
        !isAlphanumeric(inputText))
    {
        printf("Invalid input.\n");
        return 0;
    }

    /* Convert characters to binary */
    stringToBinary(inputText, data);

    /* Divide into two 8-bit blocks */
    strncpy(block1, data, 8);
    block1[8] = '\0';

    strncpy(block2, data + 8, 8);
    block2[8] = '\0';

    /* Calculate checksum */
    calculateChecksum(block1, block2, checksum);

    /* Create codeword */
    sprintf(codeword, "%s%s%s",
            block1, block2, checksum);

    printf("\nData Block 1 : %s\n", block1);
    printf("Data Block 2 : %s\n", block2);
    printf("Checksum     : %s\n", checksum);

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

    /* Extract received blocks */
    strncpy(rBlock1, received, 8);
    rBlock1[8] = '\0';

    strncpy(rBlock2, received + 8, 8);
    rBlock2[8] = '\0';

    strncpy(rChecksum, received + 16, 8);
    rChecksum[8] = '\0';

    /* Extract received data */
    strncpy(receivedData, received, 16);
    receivedData[16] = '\0';

    binaryToString(receivedData, outputText);

    /* Calculate checksum again at receiver */
    char calculatedChecksum[10];

    calculateChecksum(rBlock1, rBlock2,
                      calculatedChecksum);

    printf("\nTransmitted Codeword : %s %s %s\n",
           block1, block2, checksum);

    printf("Received Codeword    : %s %s %s\n",
           rBlock1, rBlock2, rChecksum);

    printf("Received Message     : %s\n",
           outputText);

    printf("Receiver Checksum    : %s\n",
           calculatedChecksum);

    /* Compare checksum */
    if (strcmp(calculatedChecksum, rChecksum) == 0)
        printf("Status               : No error detected!\n");
    else
        printf("Status               : Error detected!\n");

    return 0;
}
