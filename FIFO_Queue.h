//
//  FIFO_Queue.h
//  Assignment6
//


#ifndef FIFO_Queue_h
#define FIFO_Queue_h

#include <stdio.h>

// Initializing struct Event
typedef struct node{
    double start; //Holds start time of the event
    double process; //Holds the processing time of the event
    struct node *Next; //Holds the next event in the queue
}node;

typedef struct FIFO{
    node *head, *tail;
    int size;
} FIFO;

int QueueLength(FIFO *);

//Function create event
node *CreateQ(void);

//Function to add an event to the FIFO queue
void addQ(node *, FIFO *);

//Function to remove an event from the FIFO queue
void *removeQ(FIFO *);

// Function count the number of even waiting in queue
int count_all(node *);




#endif /* FIFO_Queue_h */
