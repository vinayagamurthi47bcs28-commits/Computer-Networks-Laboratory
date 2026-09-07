#include <stdio.h>

#define MAX 50


/* =========================================================
                    INPUT VALIDATION
   ========================================================= */

/* Get only 0 or 1 */
int getBinaryChoice()
{
    int choice;

    while (1)
    {
        scanf("%d", &choice);

        if (choice == 0 || choice == 1)
        {
            return choice;
        }

        printf("Invalid choice! Please enter only 0 or 1: ");
    }
}


/* =========================================================
                    STOP-AND-WAIT PROTOCOL
   ========================================================= */

void stopAndWait(int n)
{
    int i;
    int frameStatus;
    int ackStatus;

    printf("\n========================================\n");
    printf("       STOP-AND-WAIT PROTOCOL\n");
    printf("========================================\n");

    for (i = 0; i < n; i++)
    {
        while (1)
        {
            printf("\nSending Frame %d\n", i);

            /* Frame status */
            printf("Was Frame %d received successfully?\n", i);
            printf("1 - Yes\n");
            printf("0 - No (Frame Lost)\n");
            printf("Enter choice: ");

            frameStatus = getBinaryChoice();

            /* Frame lost */
            if (frameStatus == 0)
            {
                printf("Frame %d lost.\n", i);
                printf("Stop-and-Wait: Retransmitting Frame %d.\n", i);

                continue;
            }

            /* Frame received - check ACK */
            printf("\nWas ACK for Frame %d received?\n", i);
            printf("1 - Yes\n");
            printf("0 - No (ACK Lost)\n");
            printf("Enter choice: ");

            ackStatus = getBinaryChoice();

            /* ACK lost */
            if (ackStatus == 0)
            {
                printf("ACK for Frame %d lost.\n", i);
                printf("Stop-and-Wait: Retransmitting Frame %d.\n", i);

                continue;
            }

            /* ACK received */
            printf("ACK %d received.\n", i);

            break;
        }
    }

    printf("\nAll frames transmitted successfully.\n");
}


/* =========================================================
                    GO-BACK-N PROTOCOL
   ========================================================= */

void goBackN(int n, int windowSize)
{
    int base = 0;
    int i;
    int j;

    int frameStatus;
    int ackStatus;
    int lostFrame;

    printf("\n========================================\n");
    printf("       GO-BACK-N SLIDING WINDOW\n");
    printf("========================================\n");

    while (base < n)
    {
        int end = base + windowSize;

        if (end > n)
        {
            end = n;
        }

        printf("\nCurrent Window: ");

        for (i = base; i < end; i++)
        {
            printf("Frame %d ", i);
        }

        printf("\nFrames sent: ");

        for (i = base; i < end; i++)
        {
            printf("%d ", i);
        }

        printf("\n");

        lostFrame = -1;


        /* -------------------------------------------------
                    CHECK FRAME DELIVERY
           ------------------------------------------------- */

        for (i = base; i < end; i++)
        {
            printf("\nWas Frame %d received successfully?\n", i);
            printf("1 - Yes\n");
            printf("0 - No (Frame Lost)\n");
            printf("Enter choice: ");

            frameStatus = getBinaryChoice();

            /* Frame lost */
            if (frameStatus == 0)
            {
                lostFrame = i;

                printf("\nFrame %d lost.\n", i);

                printf("Go-Back-N: Retransmitting from Frame %d.\n",
                       lostFrame);

                /*
                 * Retransmit the lost frame and
                 * all following frames in window.
                 */

                for (j = lostFrame; j < end; j++)
                {
                    printf("Retransmitting Frame %d\n", j);
                }

                /*
                 * Go back to lost frame.
                 */

                base = lostFrame;

                break;
            }
        }


        /* -------------------------------------------------
                    IF FRAME WAS LOST
           ------------------------------------------------- */

        if (lostFrame != -1)
        {
            continue;
        }


        /* -------------------------------------------------
                    CHECK ACK
           ------------------------------------------------- */

        printf("\nWas ACK for the last frame (Frame %d) received?\n",
               end - 1);

        printf("1 - Yes\n");
        printf("0 - No (ACK Lost)\n");
        printf("Enter choice: ");

        ackStatus = getBinaryChoice();


        /* ACK lost */

        if (ackStatus == 0)
        {
            printf("\nACK for Frame %d lost.\n", end - 1);

            printf("Go-Back-N: ACK lost.\n");

            printf("Retransmitting current window:\n");

            for (i = base; i < end; i++)
            {
                printf("Retransmitting Frame %d\n", i);
            }

            /*
             * Window does not move.
             */

            continue;
        }


        /* ACK received */

        printf("ACK %d received.\n", end - 1);

        /*
         * Slide window.
         */

        base = end;
    }

    printf("\nAll frames transmitted successfully.\n");
}


/* =========================================================
                    SELECTIVE REPEAT PROTOCOL
   ========================================================= */

void selectiveRepeat(int n, int windowSize)
{
    int ack[MAX] = {0};

    int base = 0;
    int i;

    int frameStatus;
    int ackStatus;

    printf("\n========================================\n");
    printf("       SELECTIVE REPEAT PROTOCOL\n");
    printf("========================================\n");


    while (base < n)
    {
        int end = base + windowSize;

        if (end > n)
        {
            end = n;
        }


        /* -------------------------------------------------
                    DISPLAY CURRENT WINDOW
           ------------------------------------------------- */

        printf("\nCurrent Window: ");

        for (i = base; i < end; i++)
        {
            if (ack[i] == 0)
            {
                printf("Frame %d ", i);
            }
        }

        printf("\n");


        /* -------------------------------------------------
                    SEND FRAMES
           ------------------------------------------------- */

        for (i = base; i < end; i++)
        {
            /*
             * If already acknowledged,
             * don't send again.
             */

            if (ack[i] == 1)
            {
                continue;
            }

            printf("\nSending Frame %d\n", i);

            printf("Was Frame %d received successfully?\n", i);
            printf("1 - Yes\n");
            printf("0 - No (Frame Lost)\n");
            printf("Enter choice: ");

            frameStatus = getBinaryChoice();


            /* Frame lost */

            if (frameStatus == 0)
            {
                printf("Frame %d lost.\n", i);

                printf("Selective Repeat: Frame %d will be retransmitted.\n",
                       i);

                continue;
            }


            /* -------------------------------------------------
                        FRAME RECEIVED
               ------------------------------------------------- */

            printf("\nWas ACK for Frame %d received?\n", i);
            printf("1 - Yes\n");
            printf("0 - No (ACK Lost)\n");
            printf("Enter choice: ");

            ackStatus = getBinaryChoice();


            /* ACK received */

            if (ackStatus == 1)
            {
                ack[i] = 1;

                printf("ACK %d received.\n", i);
            }


            /* ACK lost */

            else
            {
                printf("ACK for Frame %d lost.\n", i);

                printf("Frame %d remains unacknowledged.\n", i);
            }
        }


        /* -------------------------------------------------
                    SLIDE WINDOW
           ------------------------------------------------- */

        while (base < n && ack[base] == 1)
        {
            base++;
        }


        /* -------------------------------------------------
                RETRANSMIT ONLY UNACKNOWLEDGED FRAMES
           ------------------------------------------------- */

        for (i = base; i < end; i++)
        {
            if (ack[i] == 0)
            {
                printf("\nRetransmitting Frame %d\n", i);

                printf("Was Frame %d received successfully?\n", i);
                printf("1 - Yes\n");
                printf("0 - No (Frame Lost Again)\n");
                printf("Enter choice: ");

                frameStatus = getBinaryChoice();


                /* Frame lost again */

                if (frameStatus == 0)
                {
                    printf("Frame %d lost again.\n", i);

                    continue;
                }


                /* Frame received again */

                printf("\nWas ACK for Frame %d received?\n", i);
                printf("1 - Yes\n");
                printf("0 - No (ACK Lost Again)\n");
                printf("Enter choice: ");

                ackStatus = getBinaryChoice();


                /* ACK received */

                if (ackStatus == 1)
                {
                    ack[i] = 1;

                    printf("ACK %d received.\n", i);
                }


                /* ACK lost again */

                else
                {
                    printf("ACK for Frame %d lost again.\n", i);
                }
            }
        }


        /* -------------------------------------------------
                    SLIDE WINDOW AGAIN
           ------------------------------------------------- */

        while (base < n && ack[base] == 1)
        {
            base++;
        }
    }

    printf("\nAll frames transmitted successfully.\n");
}


/* =========================================================
                           MAIN
   ========================================================= */

int main()
{
    int n;
    int windowSize;
    int choice;


    while (1)
    {
        printf("\n========================================\n");
        printf("       SLIDING WINDOW PROTOCOL\n");
        printf("========================================\n");


        /* -------------------------------------------------
                    GET NUMBER OF FRAMES
           ------------------------------------------------- */

        while (1)
        {
            printf("Enter number of frames: ");
            scanf("%d", &n);

            if (n >= 1 && n <= MAX)
            {
                break;
            }

            printf("Invalid number of frames!\n");
            printf("Please enter a value between 1 and %d.\n", MAX);
        }


        /* -------------------------------------------------
                    GET WINDOW SIZE
           ------------------------------------------------- */

        while (1)
        {
            printf("Enter window size: ");
            scanf("%d", &windowSize);

            if (windowSize >= 1 && windowSize <= n)
            {
                break;
            }

            printf("Invalid window size!\n");
            printf("Window size must be between 1 and %d.\n", n);
        }


        /* -------------------------------------------------
                    PROTOCOL MENU
           ------------------------------------------------- */

        while (1)
        {
            printf("\nChoose Protocol:\n");
            printf("1. Go-Back-N\n");
            printf("2. Selective Repeat\n");
            printf("3. Stop-and-Wait\n");
            printf("4. Enter New Frames / Window Size\n");
            printf("5. Exit\n");

            printf("Enter your choice: ");

            scanf("%d", &choice);


            switch (choice)
            {
                case 1:
                    goBackN(n, windowSize);
                    break;


                case 2:
                    selectiveRepeat(n, windowSize);
                    break;


                case 3:
                    stopAndWait(n);
                    break;


                case 4:
                    /*
                     * Leave protocol menu and
                     * enter new values.
                     */

                    goto NEW_INPUT;


                case 5:
                    printf("\nProgram terminated.\n");
                    return 0;


                default:
                    printf("\nInvalid choice!\n");
                    printf("Please enter a number between 1 and 5.\n");
            }
        }


NEW_INPUT:
        printf("\n");
    }

    return 0;
}
