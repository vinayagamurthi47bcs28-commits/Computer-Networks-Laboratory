#include <stdio.h>
#include <string.h>

char inputText[100];
char data[800];
char generator[100];
char codeword[900];
char received[900];
char remainder[100];
char temp[100];

int dataLen;
int genLen;

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

int isGoodDivisor(char *gen)
{
    int len = strlen(gen);

    if (len < 2)
        return 0;

    if (gen[0] == '1' &&
        gen[len - 1] == '1')
        return 1;

    return 0;
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
            ch = (ch << 1) |
                 (binary[i + j] - '0');
        }

        textResult[textIndex++] = ch;
    }

    textResult[textIndex] = '\0';
}

void doXor()
{
    int i;

    for (i = 1; i < genLen; i++)
    {
        temp[i - 1] =
            (temp[i] == generator[i]) ? '0' : '1';
    }
}

void calculateCRC(char *inputData)
{
    int pick;
    int i;

    strcpy(temp, inputData);

    pick = genLen;

    while (pick <= strlen(inputData))
    {
        if (temp[0] == '1')
        {
            doXor();

            temp[genLen - 1] =
                inputData[pick];
        }
        else
        {
            for (i = 0; i < genLen - 1; i++)
            {
                temp[i] = temp[i + 1];
            }

            temp[genLen - 1] =
                inputData[pick];
        }

        pick++;
    }

    strncpy(remainder,
            temp,
            genLen - 1);

    remainder[genLen - 1] = '\0';
}

void injectErrors(int numErrors, int totalLen)
{
    int pos;
    int i;

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
    int simChoice;
    int numErrors;
    int i;
    int isValid;

    char savedRemainder[100];
    char receivedPayload[800];

    printf("=== CRC METHOD ===\n");

    printf("\nEnter alphanumeric message: ");
    scanf("%s", inputText);

    if (!isAlphanumeric(inputText))
    {
        printf("Invalid input.\n");
        return 0;
    }

    printf("Enter generator polynomial: ");
    scanf("%s", generator);

    /* Validate generator */
    if (isGoodDivisor(generator))
    {
        printf("Divisor Status : GOOD Divisor Polynomial\n");
    }
    else
    {
        printf("Divisor Status : NOT GOOD Divisor\n");
        printf("Generator must start and end with 1.\n");
        return 0;
    }

    /* Convert message to binary */
    stringToBinary(inputText, data);

    dataLen = strlen(data);
    genLen = strlen(generator);

    /* Append zeros */
    strcpy(codeword, data);

    for (i = 0; i < genLen - 1; i++)
    {
        codeword[dataLen + i] = '0';
    }

    codeword[dataLen + genLen - 1] = '\0';

    /* Calculate CRC */
    calculateCRC(codeword);

    strcpy(savedRemainder, remainder);

    /* Create final codeword */
    strcpy(codeword, data);
    strcat(codeword, savedRemainder);

    printf("\nData Block : %s\n", data);
    printf("CRC Bits   : %s\n", savedRemainder);

    printf("\n1. Error-Free\n");
    printf("2. Introduce Errors\n");
    printf("Select: ");
    scanf("%d", &simChoice);

    strcpy(received, codeword);

    if (simChoice == 2)
    {
        printf("How many bits to flip? ");
        scanf("%d", &numErrors);

        injectErrors(numErrors,
                     strlen(codeword));
    }

    /* Calculate CRC at receiver */
    calculateCRC(received);

    /* Extract received data */
    strncpy(receivedPayload,
            received,
            dataLen);

    receivedPayload[dataLen] = '\0';

    binaryToString(receivedPayload,
                   inputText);

    printf("\nTransmitted Codeword : %s\n",
           codeword);

    printf("Received Codeword    : %s\n",
           received);

    printf("Received Message     : %s\n",
           inputText);

    printf("Receiver Remainder   : %s\n",
           remainder);

    /* Check whether remainder is zero */
    isValid = 1;

    for (i = 0; i < genLen - 1; i++)
    {
        if (remainder[i] != '0')
        {
            isValid = 0;
            break;
        }
    }

    if (isValid)
        printf("Status               : No error detected!\n");
    else
        printf("Status               : Error detected!\n");

    return 0;
}
