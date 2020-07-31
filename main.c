#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "import.h"
#include "engine.h"
#include "FIFO_Queue.h"
#include "Random_Generator.h"

int main(int argc, char* argv[]) {
    srand(time(NULL));
    if (argc < 4) {printf("Not enough command line arguments"); exit(1);} //Checks if all three arguments were given

    double startT = 0;
    double endT = atof(argv[1]);

    if (endT == 0) {printf("Invalid End Time"); exit(1);} //Gives this error if end time is 0 or invalid data type

    printf("Starting to import...\n");

    output* outputs = generate(argv[2]);
    comp* components = outputs->components;
    int length = outputs->length;
    FIFO* queues = outputs->queues;

    //Prints the components
    for (int i=0; i<length; i++) {
        printf("Index=%d, Type=%c, Length=%d", i, components[i].type, components[i].length);
        if (strcmp(&components[i].type, "G")==0) {printf(", Prob=[%lf], Dest=[%d]", components[i].prob[0], components->dest[0]);}
        else if (strcmp(&components[i].type, "Q")==0) {
            printf(", Prob=[");
            for (int j=0; j<components[i].length; j++) {
                printf("%lf, ", components[i].prob[j]);
            }
            printf("], Dest=[");
            for (int j=0; j<components[i].length; j++) {
                printf("%d, ", components[i].dest[j]);
            }
            printf("]");
        }

        printf("\n");
    }


    results *result = RunSim(endT, outputs);

    printf("Simulation Finished \n");

    analyze(argv[3], result->stack, components, length, result->waits, result->totalEntered);

    return 0;
}