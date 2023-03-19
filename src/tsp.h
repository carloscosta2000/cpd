#ifndef _TSP_H
#define _TSP_H

#include <stdio.h>

typedef struct bestTourPair {
    int* bestTour;
    double bestTourCost;
} bestTourPair;

typedef struct queue_element {
    int* tour;
    double cost;
    double lb;
    int length;
    int city;
} queue_element;

queue_element *queueElementCreate(int(* tour), double cost, double lb, int length, int city);

void bestTourPairDelete(bestTourPair *btPair);

//Create and initialize the BestTourPair
bestTourPair *bestTourPairCreate(int *bestTour, double bestTourCost);

//returns pair with the tour path(starting at 0 and ending at 0) and the Cost
bestTourPair *TSPBB(double(** distances), int n, double bestTourCost);

double calculateLB(double ** distances, int n);

void findTwoSmallest(double *edges, int n, double *smallests);

void* updateTour(int (*newTour), int (*tour), int length);

int checkInTour(int (*tour), int city, int length);

double calculateNewLB(double(** distances),queue_element* city_from, int city_to, int length);

void print_matrix(double** distances, int n);

void freeDistances(double **distances, int n);

#endif
