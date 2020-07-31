//
//  engine.h
//  Assignment6
//
//

#ifndef engine_h
#define engine_h

#include <stdio.h>
#include "import.h"
#include "Priority_Queue.h"

typedef struct analysis {
    int totalEntered;
    int totalExited;
    double minSystem;
    double maxSystem;
    double avgSystem;
    double minQueue;
    double maxQueue;
    double avgQueue;
} analysis;

typedef struct results {
    int totalEntered;
    double ** waits;
    LIFO *stack;
} results ;

double CurrentTime (void);

results *RunSim(double EndTime, output* outputs);

analysis *analyze(char filename[], LIFO* stack, comp *components, int length, double **waits, int totalEntered);

#endif /* engine_h */

