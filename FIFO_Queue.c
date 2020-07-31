//
//  FIFO_Queue.c
//  Assignment6
//
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "FIFO_Queue.h"


node *CreateQ(void){
    node *event=( node *) malloc(sizeof( node));
    if (event == NULL) {printf("Could not allocate memory for Queue"); exit(1);}
    event->Next = NULL;
    return event;
}

int QueueLength(FIFO *q) {
    return q->size;
}


void addQ(node *a,  FIFO *q){
    if (q->size == 0){
        q->head=q->tail=a;
    }
    else{
        q->tail->Next=a;
        q->tail=a;
    }
    q->size++;
}

void *removeQ(FIFO *q){
    void *data=NULL;
    //If queue is empty print an error
    if (q->size==0) {
        printf("Empty Queue\n");
        exit(1);
    }
    //Create a dummy pointer to the event to be removed
    node *node = q->head;
    //Move highest priority pointer to next entry
    q->head=node->Next;
    if (node==q->tail){
        q->tail=NULL;
    }
    q->size--;
    free(node);
    return(data);
}

