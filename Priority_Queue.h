//
//  Priority_Queue.h
//  Assignment6
//
//

#ifndef Priority_Queue_h
#define Priority_Queue_h

#include <stdio.h>
#include "import.h"
#include "FIFO_Queue.h"

typedef struct Customer{
    double create_time;  // the time when the source generates this part
    double finish_time;  // the time when the sink gets the part
    double total_queue; // the total time the customer spent in queues
    double enqueue_time, dequeue_time;  // records the enqueue and dequeue time at each station

} Customer;

typedef struct Event{
    double timestamp; //This is basically the priority of the event (when it occurs)
    int station_id; //Station ID
    Customer *cust;
    int NextStop;
    struct Event *Next;
}Event;

typedef struct LIFO {
    Customer *cust;
    struct LIFO *next;
} LIFO ;

LIFO *createLIFO(void);

Customer *createCust(void);

Event *createEvent();

//Function to add and event to the queue
Event *addPQ(Event *, Event *);

//Function to remove smallest timestamped event
Event *removePQ(Event *);


//Function to print timestamps of events in event list
void print_PQ(Event *);

//Assigns the destination station for the customer
int AssignStation(int N, double problist[]);

//Function that handles the events of the priority queue and FIFO queue all in one
Event *handle(Event *PrioQ, comp *components, FIFO *queues, LIFO *stack, double **waits, double currentTime);

#endif /* Priority_Queue_h */
