#ifndef _TSP_H
#define _TSP_H

#include <stdio.h>

typedef struct bestTourPair {
    int* bestTour;
    double bestTourCost;
} bestTourPair;

//Create and initialize the BestTourPair
bestTourPair *BestTourPairCreate();

//returns pair with the tour path(starting at 0 and ending at 0) and the Cost
bestTourPair *TSPBB(int(** distances), int n, double bestTourCost);

double calculateLB(int(** distances), int n);

void findTwoSmallest(int *edges, int n, int *smallests);

#endif
