#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Declare global variables
int n;
float twt_fcfs;
float twt_sjf;
float twt_rr;
float thp_fcfs;
float thp_sjf;
float thp_rr;

// Function declaration
float FCFS(int p_id[], int p_at[], int p_bt[]);
float SJF(int p_id[], int p_at[], int p_bt[]);
float RR(int p_id[], int p_at[], int p_bt[], int tq);
int print(int p_id[], int p_at[], int p_bt[], int p_ct[], int p_tat[], int p_wt[], float wt_avg, float tat_avg, int p_order[]);

struct node {
    int idx;
    struct node *next;
};

// Function to ensure input is a positive integer
int getPositiveInteger()
{
    int num;
    do
        {
            printf("Enter a positive integer: ");
            scanf("%d", &num);
            if (num <= 0)
            {
                printf("Invalid input! Please enter a positive integer.\n");
            }
        } while (num <= 0);
    return num;
}

// Function to find index of process with minimum remaining time
int minimum(int p_remainingtime[], int p_arrived[], int time)
 {
    int idx = -1;
    for (int i = 0; i < n; i++)
        {
            if (p_arrived[i] == 1 && (idx == -1 || p_remainingtime[i] < p_remainingtime[idx]))
            {
                idx = i;
            }
        }
    return idx;
}

// Function to print the execution table
int print(int p_id[], int p_at[], int p_bt[], int p_ct[], int p_tat[], int p_wt[], float wt_avg, float tat_avg, int p_order[])
 {
        printf("\n--------------------------------------------------------------------------------------------------------------------------------");
        printf("\nExecution Table is: \n");
        printf("Process ID\t\tArrival Time\t\tBurst Time\t\tCompletion Time\t\tTAT\t\tWait Time\n");
        for (int i = 0; i < n; i++)
            {
                printf("  %d\t\t\t  %d\t\t\t  %d\t\t\t\t%d\t\t %d\t\t  %d\n", p_id[i], p_at[i], p_bt[i], p_ct[i], p_tat[i], p_wt[i]);
            }
        printf("\nThe order of execution is: P%d", p_order[0]);
        for (int i = 1; i < n; i++)
         {
            printf(" -> P%d", p_order[i]);
         }
        printf("\n--------------------------------------------------------------------------------------------------------------------------------\n");
}

// Function to calculate First Come First Serve
float FCFS(int p_id[], int p_at[], int p_bt[])
 {
    int p_order[n], p_ct[n], p_tat[n], p_wt[n], time = p_at[0]; // Initialize time to the arrival time of the first process
    float wt_avg = 0, tat_avg = 0;

    // Sort processes based on arrival time
    for (int i = 0; i < n - 1; i++)
        {
        for (int j = 0; j < n - i - 1; j++)
         {
            if (p_at[j] > p_at[j + 1])
             {
                // Swap arrival time
                int temp = p_at[j];
                p_at[j] = p_at[j + 1];
                p_at[j + 1] = temp;

                // Swap process ID
                temp = p_id[j];
                p_id[j] = p_id[j + 1];
                p_id[j + 1] = temp;

                // Swap burst time
                temp = p_bt[j];
                p_bt[j] = p_bt[j + 1];
                p_bt[j + 1] = temp;
             }
          }
        }

    // Perform FCFS scheduling
    for (int i = 0; i < n; i++)
        {
            p_order[i] = p_id[i];
            time = time + p_bt[i];
            p_ct[i] = time;
                if (i != n - 1 && time < p_at[i + 1])
                {
                    time = p_at[i + 1];
                }
            p_tat[i] = p_ct[i] - p_at[i];
            p_wt[i] = p_tat[i] - p_bt[i];
            wt_avg += p_wt[i];
            tat_avg += p_tat[i];
    }
    print(p_id, p_at, p_bt, p_ct, p_tat, p_wt, wt_avg, tat_avg, p_order);
    printf("\nAverage Turn Around Time (TAT) = %f\n", tat_avg / n);
    printf("Average Wait Time (WT) = %f\n", wt_avg / n);
    twt_fcfs = wt_avg / n;
    thp_fcfs = n / (float)time;
    printf("Throughput Time = %f\n", thp_fcfs);
    return twt_fcfs;
}

// Function to calculate Shortest Job First
float SJF(int p_id[], int p_at[], int p_bt[])
{
    int p_remainingtime[n], p_order[n], p_ct[n], p_tat[n], p_wt[n], time = p_at[0], p_arrived[n], idx = 0;
    float wt_avg = 0, tat_avg = 0;

    // Sort processes based on arrival time
    for (int i = 0; i < n - 1; i++)
        {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (p_at[j] > p_at[j + 1])
             {
                // Swap arrival time
                int temp = p_at[j];
                p_at[j] = p_at[j + 1];
                p_at[j + 1] = temp;

                // Swap process ID
                temp = p_id[j];
                p_id[j] = p_id[j + 1];
                p_id[j + 1] = temp;

                // Swap burst time
                temp = p_bt[j];
                p_bt[j] = p_bt[j + 1];
                p_bt[j + 1] = temp;
            }
        }
    }

    // Initialize remaining time for each process
    for (int i = 0; i < n; i++)
    {
        p_remainingtime[i] = p_bt[i];
    }

    // Perform SJF scheduling
    for (int i = 0; i < n; i++)
        {
        if (i != n - 1 && time < p_at[i])
        {
            time = p_at[i];
        }
        // Checking for arrived processes
        for (int i = 0; i < n; i++)
        {
            if (p_at[i] <= time)
            {
                p_arrived[i] = 1;
            }
            else
            {
                p_arrived[i] = 0;
            }
        }

        idx = minimum(p_remainingtime, p_arrived, time);
        p_order[i] = p_id[idx];
        time += p_bt[idx];
        p_ct[idx] = time;
        p_tat[idx] = p_ct[idx] - p_at[idx];
        p_wt[idx] = p_tat[idx] - p_bt[idx];
        wt_avg += p_wt[idx];
        tat_avg += p_tat[idx];
        p_remainingtime[idx] = INT_MAX;
    }

    print(p_id, p_at, p_bt, p_ct, p_tat, p_wt, wt_avg, tat_avg, p_order);
    printf("\nAverage Turn Around Time (TAT) = %f\n", tat_avg / n);
    printf("Average Wait Time (WT) = %f\n", wt_avg / n);
    twt_sjf = wt_avg / n;
    thp_sjf = n / (float)time;
    printf("Throughput Time = %f\n", thp_sjf);
    return twt_sjf;
}

// Function to calculate Round Robin
float RR(int p_id[], int p_at[], int p_bt[], int tq)
 {
    int idx, p_order[n], p_ct[n], p_tat[n], p_wt[n], time = p_at[0], p_arrived[n], t = 0, completed = tq, p_remainingtime[n];
    float wt_avg = 0, tat_avg = 0;
    struct node *head, *tail;
    struct node *temp;

    head = NULL;
    tail = NULL;

    for (int i = 0; i < n; i++)
        {
            p_remainingtime[i] = p_bt[i];
        }

    p_arrived[0] = 1;

    temp = (struct node *)malloc(sizeof(struct node));
    temp->idx = 0;
    temp->next = NULL;
    head = temp;
    tail = temp;

    while (t != n)
        {
        time = time + 1;
        // Checking for arrived processes
        for (int i = 1; i < n; i++)
         {
            if (p_at[i] <= time)
            {
                if (p_arrived[i] != 1)
                {
                    p_arrived[i] = 1;
                    struct node *temp;
                    temp = (struct node *)malloc(sizeof(struct node));
                    temp->idx = i;
                    temp->next = NULL;
                    if (head == NULL)
                    {
                        head = temp;
                        tail = temp;
                    }
                    else
                    {
                        tail->next = temp;
                        tail = temp;
                    }
                }
            }
            else
            {
                p_arrived[i] = 0;
            }
        }
        if (head != NULL)
            {
                p_remainingtime[head->idx] = p_remainingtime[head->idx] - 1;
                completed = completed - 1;

            if (completed == 0 && p_remainingtime[head->idx] != 0)
            {
                struct node *temp;
                temp = (struct node *)malloc(sizeof(struct node));
                temp->idx = head->idx;
                temp->next = NULL;
                tail->next = temp;
                tail = temp;
                head = head->next;
                completed = tq;
            }
            else if (p_remainingtime[head->idx] == 0)
            {
                p_order[t] = p_id[head->idx];
                p_ct[head->idx] = time;
                p_tat[head->idx] = p_ct[head->idx] - p_at[head->idx];
                p_wt[head->idx] = p_tat[head->idx] - p_bt[head->idx];
                wt_avg += p_wt[head->idx];
                tat_avg += p_tat[head->idx];
                head = head->next;
                t = t + 1;
                completed = tq;
            }
        }
    }

    print(p_id, p_at, p_bt, p_ct, p_tat, p_wt, wt_avg, tat_avg, p_order);
    printf("\nAverage Turn Around Time (TAT) = %f\n", tat_avg / n);
    printf("Average Wait Time (WT) = %f\n", wt_avg / n);
    twt_rr = wt_avg / n;
    thp_rr = n / (float)time;
    printf("Throughput Time = %f\n", thp_rr);
    return twt_rr;
}

int main()
{
    printf("\n--------------------------------------------------------------------------------------------------------------------------------");
    printf("\t\t\t\t\t**[CPU Scheduling Algorithms Simulation]**");
    printf("\n--------------------------------------------------------------------------------------------------------------------------------");

    // Get input for the number of processes
    printf("\n\nEnter the number of processes: ");
    n = getPositiveInteger();

    int p_id[n], p_at[n], p_bt[n], tq;

    // Get input for process details
    for (int i = 0; i < n; i++)
    {
        printf("\nProcess %d\n", i + 1);
        printf("Process ID: ");
        scanf("%d", &p_id[i]);
    }
    for (int i = 0; i < n; i++)
    {
        printf("\nArrival Time of process %d: ", i + 1);
        scanf("%d", &p_at[i]);
    }
    for (int i = 0; i < n; i++)
    {
        printf("\nBurst Time of process %d: ", i + 1);
        scanf("%d", &p_bt[i]);
    }

    //Calculate FCFS, SJF, and RR
    printf("\n\t\t\t\t\t**** [FCFS] ****\n");
    twt_fcfs = FCFS(p_id, p_at, p_bt);

    printf("\n\t\t\t\t\t**** [SJF] ****\n");
    twt_sjf = SJF(p_id, p_at, p_bt);

    printf("\nEnter the Time Quanta for Round Robin Scheduling: ");
    tq = getPositiveInteger();
    printf("\n\t\t\t\t\t**** [RR] ****\n");
    twt_rr = RR(p_id, p_at, p_bt, tq);

    // Compare and print which algorithm has the least average waiting time
    printf("\nFCFS Waiting Time: %f\n", twt_fcfs);
    printf("SJF Waiting Time: %f\n", twt_sjf);
    printf("RR Waiting Time: %f\n", twt_rr);

    if (twt_fcfs < twt_sjf && twt_fcfs < twt_rr)
    {
        printf("\nFCFS has the least average waiting time.\n");
    }
    else if (twt_sjf < twt_fcfs && twt_sjf < twt_rr)
    {
        printf("\nSJF has the least average waiting time.\n");
    }
    else
    {
        printf("\nRR has the least average waiting time.\n");
    }

    // Print Throughput Time for all three algorithms
    printf("\nThroughput Time for FCFS: %f\n", thp_fcfs);
    printf("Throughput Time for SJF: %f\n", thp_sjf);
    printf("Throughput Time for RR: %f\n", thp_rr);

    return 0;
}
