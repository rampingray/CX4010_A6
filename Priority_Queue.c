//
//  Priority_Queue.c
//  Assignment6
//
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Priority_Queue.h"
#include "Random_Generator.h"

LIFO *createLIFO(void) {
    LIFO *stack;
    if ((stack = malloc (sizeof (struct LIFO))) == NULL){
        printf("Memory allocation failed\n");
        exit(1);}
    return stack;
}

void print_PQ(Event *PrioQ){
    if(PrioQ == NULL){
        printf("Queue is empty");
    }
    else{
        Event *ptr = PrioQ;
        printf ("Event List: ");
        while(ptr != NULL && ptr->timestamp !=0){
            printf("%f\n", ptr->timestamp);
            ptr=ptr->Next;
        }
    }
    printf ("\n");
}

Event *createEvent(){
    Event *e;
    if ((e = malloc (sizeof (struct Event))) == NULL){
        printf("Memory allocation failed\n");
        exit(1);}
    //printf ("Initial event list:\n");
    return e;
}

Event *addPQ(Event *event, Event *PrioQ){
    Event *e;
    e = event;
    if (PrioQ == NULL){
        PrioQ=e;
        e->Next=NULL;
    }
    else if (e->timestamp <= PrioQ->timestamp){//event e has the smallest timestamp within the Priority Queue
        e->Next=PrioQ;
        PrioQ=e;
    }
    else{
        Event *temp = PrioQ;
        while(temp->Next != NULL && e->timestamp>=temp->Next->timestamp){
            temp=temp->Next;
        }
        e->Next=temp->Next;
        temp->Next=e;
    }
    return PrioQ;
}


Event *removePQ (Event *PrioQ){
    Event *ptr;
    if (PrioQ == NULL){//Empty Queue
        printf("The Queue is empty\n");
        exit(1);
    }
    ptr=PrioQ;//Pointer points to the event that should be removed
    PrioQ=ptr->Next;
    free(ptr);
    return PrioQ;
}

Customer *createCust(void) {
    Customer *newCust;
    if ((newCust = (Customer *) malloc(sizeof(Customer))) == NULL) {
        printf("Memory Allocation Failed");
        exit(1);
    }
    newCust->total_queue = 0;
    return newCust;
}

//Assign next stop to the Event
int AssignStation(int N, double *prob){ //N is the number of different probabilities
    double random_prob = urand();
    double sum = prob[0];
    int i;
    for (i = 0; i< N; i++){
        if (sum >=random_prob){
            return i;
            break;
        }
        sum=sum+prob[i+1];
    }
    return i;
}

Event *handle(Event *PrioQ, comp *components, FIFO *queues, LIFO *stack, double **waits, double currentTime) {
    //Attains the current status (Need to update average wait time)
    //Removes from current FIFO queue
    //Adds to new FIFO queue or to END
    //Adds new Event in PrioQ unless added to end
    //Removes the Event from PrioQ

    //Checks for destination type Exit
    if ( (int) components[PrioQ->NextStop].type == 69) {
        //NextStop is Type: Exit

        printf("Item went to the exit \n");
        waits[PrioQ->station_id][0] = (waits[PrioQ->station_id][0] * waits[PrioQ->station_id][1] + (PrioQ->cust->dequeue_time - PrioQ->cust->enqueue_time)) / (waits[PrioQ->station_id][1] + 1);
        waits[PrioQ->station_id][1] += 1; //Increments the number of customers who have passed through the given queue
        removeQ(&queues[PrioQ->station_id]); //Removes the old event from the FIFO queue
        PrioQ->cust->total_queue += (PrioQ->cust->dequeue_time - PrioQ->cust->enqueue_time); // Updates the customer's total queue wait time

        PrioQ->cust->finish_time = currentTime; //Writes the customer's finish time as the current system time
        LIFO *newLIFO = createLIFO(); //Creates a new stack element
        newLIFO->cust = PrioQ->cust; //Fills the stack with customer data
        newLIFO->next = stack->next; //Puts newLIFO at the front of the stack
        stack->next = newLIFO; //Changes stack to point at the new front

    } else if ( (int) components[PrioQ->NextStop].type == 81) {
        //NextStop is Type: Queue

        printf("Item added to another queue \n");
        waits[PrioQ->station_id][0] = (waits[PrioQ->station_id][0] * waits[PrioQ->station_id][1] + (PrioQ->cust->dequeue_time - PrioQ->cust->enqueue_time)) / (waits[PrioQ->station_id][1] + 1);
        waits[PrioQ->station_id][1] += 1; //Increments the number of customers who have passed through the given queue
        removeQ(&queues[PrioQ->station_id]); //Removes the old event from the FIFO queue
        PrioQ->cust->total_queue += (PrioQ->cust->dequeue_time - PrioQ->cust->enqueue_time); // Updates the customer's total queue wait time

        node *newNode = CreateQ(); //Creates the new FIFO queue event

        //Inserts data into newNode
        //Checks if there is anyone currently in the destination queue
        if (queues[PrioQ->NextStop].size >= 1) {
            newNode->start = queues[PrioQ->NextStop].tail->start + queues[PrioQ->NextStop].tail->process; //Sets time to start after last item in destination queue
            if (currentTime > newNode->start) {newNode->start = currentTime;}
        } else {newNode->start = currentTime;} //If no item in queue, start time is the current time
        newNode->process = randexp(components[PrioQ->NextStop].rate);
        addQ(newNode, &queues[PrioQ->NextStop]); //Adds newNode to its respective new location

        Event *newEvent = createEvent(); //Creates the new Event for the PQ
        //Inserts data into newEvent
        newEvent->timestamp = newNode->process + newNode->start; //Timestamp is the start time + process time
        newEvent->cust = PrioQ->cust; //newEvent gets the pointer to the old customer
        newEvent->station_id = PrioQ->NextStop;
        int nextStopIndex = AssignStation(components[PrioQ->NextStop].length, components[PrioQ->NextStop].prob);
        newEvent->NextStop = components[PrioQ->NextStop].dest[nextStopIndex];

        newEvent->cust->enqueue_time = currentTime;
        newEvent->cust->dequeue_time = newNode->start;

        PrioQ = addPQ(newEvent, PrioQ);

    } else if ( (int) components[PrioQ->NextStop].type == 71) {
        //NextStop is Type: Generator (Means Newly Generated Item)

        Event *newEvent = createEvent(); //Creates the new Event for the PQ
        newEvent->station_id = components[PrioQ->NextStop].dest[0]; //Next stop is already determined
        newEvent->cust = PrioQ->cust; //Carries over the customer structure
        int nextStopIndex = AssignStation(components[newEvent->station_id].length, components[newEvent->station_id].prob);
        newEvent->NextStop = components[newEvent->station_id].dest[nextStopIndex];

        node *newNode = CreateQ(); //Creates the new FIFO queue event

        if (queues[newEvent->station_id].size >= 1) {
            newNode->start = queues[newEvent->station_id].tail->start + queues[newEvent->station_id].tail->process; //Sets time to start after last item in destination queue
            if (currentTime > newNode->start) {newNode->start = currentTime;}
        } else {newNode->start = currentTime;} //If no item in queue, start time is the current time
        double newRate = components[newEvent->station_id].rate;
        newNode->process = randexp(newRate);
        addQ(newNode, &queues[newEvent->station_id]); //Adds newNode to its respective new location

        newEvent->timestamp = newNode->start + newNode->process;
        newEvent->cust->create_time = currentTime;
        newEvent->cust->enqueue_time = currentTime;
        newEvent->cust->dequeue_time = newNode->start;

        PrioQ = addPQ(newEvent, PrioQ);

    }else {printf("Invalid Destination Type"); exit(1);}


    PrioQ = removePQ(PrioQ); //Removes and frees the first event in the PrioQ



    return PrioQ; //Returns the new first event in the PrioQ
}
