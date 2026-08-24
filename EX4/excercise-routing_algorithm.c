#include <stdio.h>
#include <string.h>

#define MAX 10
#define INF 999

int n;

char name[MAX][20];

/* Direct link cost */
int linkCost[MAX][MAX];

/* Shortest distance between routers */
int cost[MAX][MAX];

/* Next router in the shortest path */
int nextHop[MAX][MAX];


/* =========================================================
   Find router index using router name
   ========================================================= */
int findRouter(char rname[])
{
    for (int i = 0; i < n; i++)
    {
        if (strcmp(name[i], rname) == 0)
            return i;
    }

    return -1;
}


/* =========================================================
   Display routing table
   ========================================================= */
void displayMatrix(const char *title)
{
    printf("\n\n========================================================\n");
    printf(" %s\n", title);
    printf("========================================================\n");

    printf("\n%-12s", "Router");

    for (int j = 0; j < n; j++)
        printf("%-8s", name[j]);

    printf("\n");
    printf("--------------------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        printf("%-12s", name[i]);

        for (int j = 0; j < n; j++)
        {
            if (cost[i][j] >= INF)
                printf("%-8s", "INF");
            else
                printf("%-8d", cost[i][j]);
        }

        printf("\n");
    }
}


/* =========================================================
   Initialize routing tables
   ========================================================= */
void initializeTables()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            /* Start with direct link costs */
            cost[i][j] = linkCost[i][j];

            if (i == j)
            {
                nextHop[i][j] = i;
            }
            else if (linkCost[i][j] < INF)
            {
                /* Direct neighbor */
                nextHop[i][j] = j;
            }
            else
            {
                nextHop[i][j] = -1;
            }
        }
    }
}


/* =========================================================
   Distance Vector Routing Algorithm
   ========================================================= */
void distanceVector()
{
    int updated;

    do
    {
        updated = 0;

        /* Router i */
        for (int i = 0; i < n; i++)
        {
            /* Neighbor j */
            for (int j = 0; j < n; j++)
            {
                /*
                   j must be a direct neighbor of i.
                */
                if (i == j || linkCost[i][j] >= INF)
                    continue;

                /* Destination k */
                for (int k = 0; k < n; k++)
                {
                    if (i == k)
                        continue;

                    /*
                       Check whether going through
                       neighbor j gives a shorter path.
                    */
                    if (cost[j][k] < INF &&
                        linkCost[i][j] + cost[j][k] < cost[i][k])
                    {
                        cost[i][k] =
                            linkCost[i][j] + cost[j][k];

                        /*
                           First hop from i to k
                           is router j.
                        */
                        nextHop[i][k] = j;

                        updated = 1;
                    }
                }
            }
        }

    } while (updated);
}


/* =========================================================
   Read input
   ========================================================= */
void readInput()
{
    printf("========================================================\n");
    printf(" DISTANCE VECTOR ROUTING ALGORITHM\n");
    printf("========================================================\n");

    printf("\nEnter number of routers: ");
    scanf("%d", &n);

    printf("\nEnter router names:\n");

    for (int i = 0; i < n; i++)
        scanf("%s", name[i]);

    printf("\nEnter the cost matrix:\n");
    printf("(Enter 0 for same router and -1 for no direct link)\n\n");

    for (int i = 0; i < n; i++)
    {
        printf("Enter row for %s: ", name[i]);

        for (int j = 0; j < n; j++)
        {
            int value;

            scanf("%d", &value);

            if (i == j)
            {
                linkCost[i][j] = 0;
            }
            else if (value == -1)
            {
                linkCost[i][j] = INF;
            }
            else
            {
                linkCost[i][j] = value;
            }
        }
    }
}


/* =========================================================
   Update an existing edge
   ========================================================= */
void updateEdge()
{
    char node1[20];
    char node2[20];

    printf("\n\n========================================================\n");
    printf(" UPDATE EDGE\n");
    printf("========================================================\n");

    printf("\nEnter first node: ");
    scanf("%s", node1);

    printf("Enter second node: ");
    scanf("%s", node2);

    int u = findRouter(node1);
    int v = findRouter(node2);

    if (u == -1 || v == -1)
    {
        printf("\nInvalid router name!\n");
        return;
    }

    if (u == v)
    {
        printf("\nA router cannot be connected to itself.\n");
        return;
    }

    printf("\nCurrent cost between %s and %s = ",
           node1, node2);

    if (linkCost[u][v] >= INF)
        printf("No direct link\n");
    else
        printf("%d\n", linkCost[u][v]);

    int newCost;

    printf("\nEnter new cost: ");
    scanf("%d", &newCost);

    if (newCost <= 0)
    {
        printf("\nInvalid cost! Cost must be greater than 0.\n");
        return;
    }

    /*
       Since the network is undirected,
       update both directions.
    */
    linkCost[u][v] = newCost;
    linkCost[v][u] = newCost;

    printf("\nEdge %s-%s updated successfully.\n",
           node1, node2);

    /*
       IMPORTANT:
       Reset the routing tables using the
       updated direct-link costs.
    */
    initializeTables();

    /*
       Run Distance Vector again.
    */
    distanceVector();

    printf("\nRouting tables recalculated successfully.\n");
}


/* =========================================================
   Find shortest path and cost
   ========================================================= */
void findShortestPath()
{
    char sourceName[20];
    char destinationName[20];

    printf("\n\n========================================================\n");
    printf(" TO FIND SHORTEST PATH\n");
    printf("========================================================\n");

    printf("\nEnter starting node: ");
    scanf("%s", sourceName);

    printf("Enter ending node: ");
    scanf("%s", destinationName);

    int source = findRouter(sourceName);
    int destination = findRouter(destinationName);

    if (source == -1)
    {
        printf("\nInvalid starting node!\n");
        return;
    }

    if (destination == -1)
    {
        printf("\nInvalid ending node!\n");
        return;
    }

    /* Same source and destination */
    if (source == destination)
    {
        printf("\nStarting Node : %s\n", sourceName);
        printf("Ending Node : %s\n", destinationName);
        printf("Shortest Path : %s\n", sourceName);
        printf("Total Cost : 0\n");

        return;
    }

    /* No path */
    if (cost[source][destination] >= INF)
    {
        printf("\nNo path exists between %s and %s.\n",
               sourceName, destinationName);

        return;
    }

    printf("\nStarting Node : %s\n", sourceName);
    printf("Ending Node : %s\n", destinationName);

    printf("Shortest Path : ");

    int current = source;

    printf("%s", name[current]);

    /*
       Follow nextHop until destination.
    */
    while (current != destination)
    {
        current = nextHop[current][destination];

        if (current == -1)
        {
            printf("\nPath cannot be determined.\n");
            return;
        }

        printf(" -> %s", name[current]);
    }

    printf("\nTotal Cost : %d\n",
           cost[source][destination]);
}


/* =========================================================
   Main
   ========================================================= */
int main()
{
    char choice;

    /* -----------------------------------------------------
       Step 1: Read network
       ----------------------------------------------------- */
    readInput();


    /* -----------------------------------------------------
       Step 2: Initialize routing tables
       ----------------------------------------------------- */
    initializeTables();


    /* -----------------------------------------------------
       Step 3: Display initial routing table
       ----------------------------------------------------- */
    displayMatrix("INITIAL ROUTING TABLE");


    /* -----------------------------------------------------
       Step 4: Run Distance Vector
       ----------------------------------------------------- */
    distanceVector();


    /* -----------------------------------------------------
       Step 5: Display final routing table
       ----------------------------------------------------- */
    displayMatrix("FINAL ROUTING TABLE");


    /*
       -----------------------------------------------------
       Step 6 onwards:
       First find shortest path.
       Then ask whether user wants to update an edge.
       -----------------------------------------------------
    */

    do
    {
        /* Find shortest path */
        findShortestPath();

        /*
           Ask for update only AFTER
           displaying shortest path.
        */
        printf("\n\nDo you want to update an edge? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y')
        {
            /* Update edge */
            updateEdge();

            /* Display new routing table */
            displayMatrix("UPDATED ROUTING TABLE");
        }

    } while (choice == 'y' || choice == 'Y');


    /* -----------------------------------------------------
       Program termination
       ----------------------------------------------------- */
    printf("\n\n========================================================\n");
    printf(" PROGRAM TERMINATED\n");
    printf("========================================================\n");

    return 0;
}
[16/08, 2:36 pm] Lokchana✨: #include <stdio.h>
#include <string.h>

#define MAX 10
#define INF 999

int n;

char name[MAX][20];

/* Direct link cost */
int cost[MAX][MAX];

/* Shortest distance from each router */
int dist[MAX][MAX];

/* Next router in shortest path */
int nextHop[MAX][MAX];


/* =========================================================
   Find router index using router name
   ========================================================= */
int findRouter(char rname[])
{
    for (int i = 0; i < n; i++)
    {
        if (strcmp(name[i], rname) == 0)
            return i;
    }

    return -1;
}


/* =========================================================
   Display routing matrix
   ========================================================= */
void displayMatrix(const char *title, int mat[MAX][MAX])
{
    printf("\n\n========================================================\n");
    printf(" %s\n", title);
    printf("========================================================\n");

    printf("\n%-12s", "Router");

    for (int j = 0; j < n; j++)
        printf("%-8s", name[j]);

    printf("\n");
    printf("--------------------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        printf("%-12s", name[i]);

        for (int j = 0; j < n; j++)
        {
            if (mat[i][j] >= INF)
                printf("%-8s", "INF");
            else
                printf("%-8d", mat[i][j]);
        }

        printf("\n");
    }
}


/* =========================================================
   Dijkstra's Algorithm
   Runs from source router s
   ========================================================= */
void dijkstra(int s)
{
    int visited[MAX] = {0};

    /* Initialize */
    for (int i = 0; i < n; i++)
    {
        dist[s][i] = cost[s][i];

        if (i != s && cost[s][i] < INF)
            nextHop[s][i] = i;
        else
            nextHop[s][i] = -1;
    }

    dist[s][s] = 0;
    nextHop[s][s] = s;

    /* Source is visited */
    visited[s] = 1;

    /* Find shortest paths */
    for (int count = 1; count < n; count++)
    {
        int u = -1;
        int minDist = INF;

        /* Find unvisited router with minimum distance */
        for (int i = 0; i < n; i++)
        {
            if (!visited[i] && dist[s][i] < minDist)
            {
                minDist = dist[s][i];
                u = i;
            }
        }

        /* No more reachable routers */
        if (u == -1)
            break;

        visited[u] = 1;

        /* Relax edges from u */
        for (int v = 0; v < n; v++)
        {
            if (!visited[v] &&
                cost[u][v] < INF &&
                dist[s][u] + cost[u][v] < dist[s][v])
            {
                dist[s][v] =
                    dist[s][u] + cost[u][v];

                /*
                   The first hop from source s
                   is the same first hop used
                   to reach u.
                */
                nextHop[s][v] = nextHop[s][u];
            }
        }
    }
}


/* =========================================================
   Run Dijkstra for every router
   ========================================================= */
void runLinkState()
{
    for (int i = 0; i < n; i++)
        dijkstra(i);
}


/* =========================================================
   Initialize Dijkstra tables
   ========================================================= */
void initializeTables()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            dist[i][j] = INF;
            nextHop[i][j] = -1;
        }
    }
}


/* =========================================================
   Read network input
   ========================================================= */
void readInput()
{
    printf("========================================================\n");
    printf(" LINK STATE ROUTING ALGORITHM\n");
    printf("========================================================\n");

    printf("\nEnter number of routers: ");
    scanf("%d", &n);

    printf("\nEnter router names:\n");

    for (int i = 0; i < n; i++)
        scanf("%s", name[i]);

    /*
       Read cost matrix.
       -1 means no direct link.
    */
    printf("\nEnter the cost matrix:\n");
    printf("(Enter 0 for same router and -1 for no direct link)\n\n");

    for (int i = 0; i < n; i++)
    {
        printf("Enter row for %s: ", name[i]);

        for (int j = 0; j < n; j++)
        {
            int value;

            scanf("%d", &value);

            if (i == j)
            {
                cost[i][j] = 0;
            }
            else if (value == -1)
            {
                cost[i][j] = INF;
            }
            else
            {
                cost[i][j] = value;
            }
        }
    }
}


/* =========================================================
   Update an edge
   ========================================================= */
void updateEdge()
{
    char node1[20];
    char node2[20];

    printf("\n\n========================================================\n");
    printf(" UPDATE EDGE\n");
    printf("========================================================\n");

    printf("\nEnter first node: ");
    scanf("%s", node1);

    printf("Enter second node: ");
    scanf("%s", node2);

    int u = findRouter(node1);
    int v = findRouter(node2);

    if (u == -1 || v == -1)
    {
        printf("\nInvalid router name!\n");
        return;
    }

    if (u == v)
    {
        printf("\nA router cannot be connected to itself.\n");
        return;
    }

    printf("\nCurrent cost between %s and %s = ",
           node1, node2);

    if (cost[u][v] >= INF)
        printf("No direct link\n");
    else
        printf("%d\n", cost[u][v]);

    int newCost;

    printf("\nEnter new cost: ");
    scanf("%d", &newCost);

    if (newCost <= 0)
    {
        printf("\nInvalid cost! Cost must be greater than 0.\n");
        return;
    }

    /*
       Network is undirected,
       so update both directions.
    */
    cost[u][v] = newCost;
    cost[v][u] = newCost;

    printf("\nEdge %s-%s updated successfully.\n",
           node1, node2);

    /*
       Reset old shortest-path information.
    */
    initializeTables();

    /*
       Run Dijkstra again for every router.
    */
    runLinkState();

    printf("\nRouting tables recalculated successfully.\n");
}


/* =========================================================
   Find shortest path and cost
   ========================================================= */
void findShortestPath()
{
    char sourceName[20];
    char destinationName[20];

    printf("\n\n========================================================\n");
    printf(" TO FIND SHORTEST PATH\n");
    printf("========================================================\n");

    printf("\nEnter starting node: ");
    scanf("%s", sourceName);

    printf("Enter ending node: ");
    scanf("%s", destinationName);

    int source = findRouter(sourceName);
    int destination = findRouter(destinationName);

    if (source == -1)
    {
        printf("\nInvalid starting node!\n");
        return;
    }

    if (destination == -1)
    {
        printf("\nInvalid ending node!\n");
        return;
    }

    /* Same source and destination */
    if (source == destination)
    {
        printf("\nStarting Node : %s\n", sourceName);
        printf("Ending Node : %s\n", destinationName);
        printf("Shortest Path : %s\n", sourceName);
        printf("Total Cost : 0\n");

        return;
    }

    /* No path */
    if (dist[source][destination] >= INF)
    {
        printf("\nNo path exists between %s and %s.\n",
               sourceName, destinationName);

        return;
    }

    printf("\nStarting Node : %s\n", sourceName);
    printf("Ending Node : %s\n", destinationName);

    printf("Shortest Path : ");

    int current = source;

    printf("%s", name[current]);

    /*
       Follow nextHop until destination.
    */
    while (current != destination)
    {
        current = nextHop[current][destination];

        if (current == -1)
        {
            printf("\nPath cannot be determined.\n");
            return;
        }

        printf(" -> %s", name[current]);
    }

    printf("\nTotal Cost : %d\n",
           dist[source][destination]);
}


/* =========================================================
   Main
   ========================================================= */
int main()
{
    char choice;

    /* Step 1: Read network */
    readInput();

    /* Step 2: Initialize */
    initializeTables();

    /* Step 3: Display initial routing table */
    displayMatrix("INITIAL ROUTING TABLE", cost);

    /* Step 4: Run Link State Algorithm */
    runLinkState();

    /* Step 5: Display final routing table */
    displayMatrix("FINAL ROUTING TABLE", dist);

    /*
       Step 6 onwards:
       Find shortest path FIRST.
       Then ask whether to update.
    */
    do
    {
        /* Find shortest path */
        findShortestPath();

        /*
           Ask for update AFTER
           displaying shortest path.
        */
        printf("\n\nDo you want to update an edge? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y')
        {
            /* Update edge */
            updateEdge();

            /* Display updated routing table */
            displayMatrix("UPDATED ROUTING TABLE", dist);
        }

    } while (choice == 'y' || choice == 'Y');


    /* Program termination */
    printf("\n\n========================================================\n");
    printf(" PROGRAM TERMINATED\n");
    printf("========================================================\n");

    return 0;
}
