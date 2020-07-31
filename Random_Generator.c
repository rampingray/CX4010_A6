//
//  Random_Generator.c
//  Assignment6
//
//
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Random_Generator.h"

//lambda is interarrival time=1/Mean
double randexp(double U){
    double u;
    u=urand();
    double rand = -U*log(1.0-u);
    return rand;
}

double urand(){
    return (double)rand() / (double)RAND_MAX;
}

