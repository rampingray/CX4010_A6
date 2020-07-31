#ifndef A6_IMPORT_H
#define A6_IMPORT_H

#include "FIFO_Queue.h"

typedef struct comp {
    char type; //Component type ("G", "Q", or "E")
    double rate; //Rate
    int length; //Length of prob and dest arrays NULL for E
    double *prob; //Pointer to array of probabilties of being send to the corresponding destination (same index)
    int *dest; //Pointer to array of destination IDs (indexes match up to prob array)
} comp;

typedef struct output {
    comp* components;
    FIFO* queues;
    int length;
} output;

/* Generate: Generates a pointer to the array that will store the structs for each component specified in the config file.
 * Array indices match the ID of the component from the config file.
 */
output* generate(char filepath[]);

#endif //A6_IMPORT_H