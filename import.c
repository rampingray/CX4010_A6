#include "import.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "FIFO_Queue.h"

//Outputs the length of the components array along with a pointer to the components array itself
output* generate(char filepath[]) {
    printf("Starting generate... \n"); //Starting message

    output* outputs = malloc(sizeof(output)); //Creates the outputs variable by allocating memory and storing the pointer
    if (!outputs) {printf("Failed to make outputs...\n"); return NULL;} //Checks success of malloc

    char line[1000]; //Buffer for each line to be read
    FILE* filename = fopen(filepath, "r"); //Reads the file contents to memory
    if (filename == NULL) {printf("Invalid Input Filename"); exit(1);}

    int length; //Length of the file given by the integer on the first line
    //Reads the first line of the config file (contains the length of the file)
    if (fgets(line, 1000, filename) != NULL) {
        sscanf(line, "%d", &length); //Scans the first line and stores the integer length in the length variable
    } else {return 0;} //Returns 0 if the config file is empty
    comp* components = malloc(length * sizeof(comp)); //Creates the array of length specified by the fist line of the file*
    if (!components) {printf("Failed to make components...\n"); return NULL;} //Checks success of malloc

    FIFO* queues; //Initializes the array of FIFO struct pointers
    queues = (FIFO *) malloc(length * sizeof(FIFO));
    if (queues == NULL) {printf("Memory Allocation Failed"); exit(1);}

    int hasGen = 0;
    int hasExit = 0;

    //Iterates over each line of the config file and writes the contents to the array
    for (int i=1; i<length+1; i++) {
        int index; //Initializes the int index
        char type; //Initializes the character type
        fgets(line, 1000, filename); //Reads a line of the file to the variable, line
        sscanf(line, "%d %c", &index, &type); //Scans the line for the component index and type

        //Checks the type of the component and executes the appropriate code (Checks by mapping to int and checking ASCII dec value)
        if ((int)type == 71) {
            //If the component is a generator
            hasGen++; //Increments hasGen if there is a generator present
            int length = 1; //Sets length to 1 because generators only output to one location
            double rate; //Initializes the double, rate
            int* dest = malloc(sizeof(int)); //Creates the dest array and allocates memory
            if (!dest) {return NULL;} //Returns NULL if malloc fails
            double* prob = malloc(sizeof(double)); //Creates the prob array and allocates memory
            if (!prob) {return NULL;} //Returns NULL if malloc fails
            prob[0] = 1.0; //Sets the probability to 1 because there is only one destination
            sscanf(line, "%d %c %lf %d", &index, &type, &rate, &dest[0]); //Scans for the rest of the variables
            components[i-1].dest = dest; //Stores the dest array in the components array element
            components[i-1].length=length; //Stores the length in the components array element
            components[i-1].rate=rate; //Stores the rate in the components array element
            components[i-1].type=type; //Stores the type in the components array element
            components[i-1].prob=prob; //Stores the prob array in the components array element
        } else if ((int)type == 81) {
            //If the component is a queue
            int length; //Initializes the int, length
            double rate; //Intiailizes the double, rate
            sscanf(line, "%d %c %lf %d", &index, &type, &rate, &length); //Scans for the four set variables
            double* prob = malloc(length * sizeof(double)); //Allocates memory for a prob array of length, length
            if (!prob) {return NULL;} //Returns NULL if malloc fails
            int* dest = malloc(length * sizeof(int)); //Allocates memory for a dest array of length, length
            if (!dest) {return NULL;} //Returns NULL if malloc fails
            strtok(line, " "); //Tokenizes the line
            for (int j=1; j<4; j++) {strtok(NULL, " ");} //Eats up the first few variables because they arent needed
            double runningProb = 0; //Running cumulative probability
            for (int j=4; j<length+4; j++) {
                prob[j-4] = atof(strtok(NULL, " ")); //Adds each probability to the prob array
                runningProb += prob[j-4]; //Adds the probability to the running prob
                printf("Prob: %lf \n", runningProb);
            }
            if (runningProb > 1.0 || runningProb < 0.99999) {printf("Cumulative Probs Not 1.0 on %d; %lf instead", i-1, runningProb); exit(1);} //Checks that probs sum to 1
            for (int j=length+4; j<2*length+4; j++) {
                dest[j-(length+4)] = atoi(strtok(NULL, " ")); //Adds each dest to the dest array
            }

            queues[i-1].size = 0;

            components[i-1].dest = dest; //Stores the dest array in the component array element
            components[i-1].length=length; //Stores the length in the component array element
            components[i-1].rate=rate; //Stores the rate in the component array element
            components[i-1].type=type; //Stores the type in the component array element
            components[i-1].prob=prob; //Stores the prob array in the component array element

        } else if ((int)type == 69) {
            //Exit components only need the type, so the type just gets stored if type == "E"
            hasExit++; //Increments hasExit if there is an exit
            components[i-1].type=type; //Stores the type in the component array element
        } else {printf("Failed on type: %c, %d\n", type, type); printf("Value: %d", strcmp(&type, "Q"));  exit(1);}
    }

    // Exits if there are no generators or exits in the system config file
    if (hasExit == 0 || hasGen == 0) {printf("Config missing Exit or Generator"); exit(1);}

    printf("Queues Below: \n");
    for (int i=1; i<4; i++) {
        printf("Queue %d's Size: %d \n", i, queues[i].size);
    }

    outputs->length = length; //Puts the length in the outputs structure
    outputs->components = components; //Puts the components in the outputs structure
    outputs->queues = queues; //Puts the queues in the outputs structure

    fclose(filename);
    return outputs; //Returns the outputs
}

