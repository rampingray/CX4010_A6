//
//  engine.c
//  Assignment6
//
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "engine.h"
#include "Priority_Queue.h"
#include "Random_Generator.h"


#define    ARRIVAL     1
#define    DEPARTURE   2


//Initiallize Priority Queue to NULL
Event *PrioQ = NULL;

// Simulation clock variable
double Now = 0.0;

// Return current simulation time
double CurrentTime (void)
{
    return (Now);
}

void schedule(double ts, int NextStop){
    Event *e;
    // create event data structure and fill it in
    if ((e = malloc (sizeof (struct Event))) == NULL){
        printf("Memory allocation failed\n");
        exit(1);}
    e->timestamp=ts;
    e->NextStop=NextStop;
    PrioQ=addPQ(e, PrioQ);
    print_PQ(PrioQ);
}

results *RunSim(double EndTime, output* outputs){
    results *result = (results *) malloc(sizeof(results));
    if (result == NULL) {printf("Memory allocation failed"); exit(1);}
    result->totalEntered = 0;

    //waits[i][j] i is the station ID and j==0 is the current average wait time and j==1 is the number of customers processed
    double **waits = malloc(outputs->length * sizeof(double *)); //Allocates space for the array of double pointers
    if (waits==NULL) {exit(1);} //Checks if allocation failed
    for (int i=0; i<outputs->length; i++) {
        waits[i] = (double*) malloc(2 * sizeof(double)); //Allocates the space for each double pointer element of waits
        if (waits[i] == NULL) {exit(1);} //Checks if allocation failed
    }
    //Initializes all of the values of waits to 0
    for (int i=0; i<outputs->length; i++) {
        for (int j=0; j<2; j++) {
            waits[i][j] = 0; //Sets the value of waits[i][j] to 0
        }
    }

    comp* components = outputs->components; // Creates a local variable to hold the pointer to the components array
    FIFO* queues = outputs->queues; //Creates a local variable to hold the queues array
    LIFO *stack = createLIFO(); //Creates a local variable for the END stack
    stack->next = NULL; //Initializes the next item in the stack as NULL
    int length = outputs->length; //Creates a local variable for the length

    for (int i=0; i<length;i++){
        double currentTime=0;
        if (strcmp(&components[i].type, "G")==0){
            while (currentTime < EndTime) {
                //Creates the PQ event
                Event *e=createEvent(); //Initializes the new PrioQ event
                e->station_id = i; //Current station is the ID of the generator
                e->timestamp = currentTime; //Sets the creation timestamp as the current time
                e->NextStop = i; //NextStop is the destination of the generator
                e->cust = createCust(); //Creates a new customer
                e->cust->create_time = currentTime; //Creation time is the current time

                PrioQ=addPQ(e, PrioQ); //Adds the new PQ item to the priority queue
                currentTime += randexp(components[i].rate); //Increments the currentTime by the interarrival rate
                printf("Added Event at time: %lf \n", e->timestamp);
                result->totalEntered++;
            }
        }
    }
    printf ("Initial event list:\n");
    print_PQ(PrioQ);
        // Main scheduler loop
    Event *ptr;
    while ((ptr=PrioQ) != NULL && ptr->timestamp <= EndTime) {
        Now = ptr->timestamp;
        if (Now > EndTime) break;
        PrioQ = handle(ptr, components, queues, stack, waits, Now);
        //print_PQ(PrioQ);
    }

    result->stack = stack;
    result->waits = waits;
    return result;
}

analysis *analyze(char filename[], LIFO *stack, comp *components, int length, double **waits, int totalEntered) {
    FILE *filepath = fopen(filename, "w"); //Opens a file pointer for writing
    if (filepath == NULL) {printf("Invalid Output File"); exit(1);}

    analysis *data; //Initializes analysis data
    data = (analysis *) malloc(sizeof(analysis));
    if (data == NULL) {printf("Memory allocatoin failed"); exit(1);}

    data->totalEntered = totalEntered;
    data->avgSystem = 0; //Sets all data points equal to 0
    data->maxSystem = 0;
    data->totalExited = 0;
    data->minSystem = INFINITY;
    data->avgQueue = 0;
    data->minQueue = INFINITY;
    data->maxQueue = 0;

    printf("Data Initialized\n");

    for (int i=0; i<length; i++) {
        if ( (int) components[i].type == 81) {
            fprintf(filepath, "Avg. Wait at stations %d was %lf \n", i, waits[i][0]);
        }
    }

    while (stack->next != NULL) {
        if (stack->cust != NULL) {
            //printf("Starting Loop");
            double totalSystem = stack->cust->finish_time - stack->cust->create_time;

            if (stack->cust->total_queue > data->maxQueue) { data->maxQueue = stack->cust->total_queue; }
            if (stack->cust->total_queue < data->minQueue) { data->minQueue = stack->cust->total_queue; }
            data->avgQueue = (data->totalExited * data->avgQueue + stack->cust->total_queue) / (data->totalExited + 1);
            //printf("Queues Work\n");
            if (totalSystem > data->maxSystem) { data->maxSystem = totalSystem; }
            if (totalSystem < data->minSystem && totalSystem > 0) { data->minSystem = totalSystem; }
            //totalSystem>0 because PACE cluster calculated a null value as -0.0000, so this eliminates that issue
            data->avgSystem = (data->totalExited * data->avgSystem + totalSystem) / (data->totalExited + 1);
            //printf("System Works\n");
            data->totalExited += 1;
            //printf("Total Exited Works\n");
        }
        stack = stack->next;
        //printf("Stack Iterated\n");
        //printf ("Stack: %p \n", stack);
    }

    printf("Data collected from stack\n");

    fprintf(filepath, "Customers Entered: %d \n", data->totalEntered);
    fprintf(filepath, "Customers Exited: %d \n", data->totalExited);

    fprintf(filepath, "Queue Times: ");
    fprintf(filepath, "AvgQueue: %lf, ", data->avgQueue);
    fprintf(filepath, "MinQueue: %lf, ", data->minQueue);
    fprintf(filepath, "MaxQueue: %lf", data->maxQueue);

    fprintf(filepath, "\nSystem Times: ");
    fprintf(filepath, "AvgSystem: %lf, ", data->avgSystem);
    fprintf(filepath, "MinSystem: %lf, ", data->minSystem);
    fprintf(filepath, "MaxSystem: %lf", data->maxSystem);

    free(data);

    fclose(filepath); //Closes the file
}
