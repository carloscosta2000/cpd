#ifndef _TSP_H
#define _TSP_H

#include <stdio.h>

typedef struct bestTourPair {
    int* bestTour;
    double bestTourCost;
    int finished;
} bestTourPair;

typedef struct queue_element {
    int* tour;
    double cost;
    double lb;
    int length;
    int city;
} queue_element;

char cmp(void* queue_element_1, void* queue_element_2);

queue_element *queueElementCreate(int(* tour), double cost, double lb, int length, int city);

void bestTourPairDelete(bestTourPair *btPair);

void list_queues_delete(priority_queue_t** queues);

void queue_element_delete(queue_element *e);

//Create and initialize the BestTourPair
bestTourPair *bestTourPairCreate(int *bestTour, double bestTourCost);

//Creates queues
priority_queue_t ** init_list_queues(int num_threads);

//Adds 0's neighbours to workers queues.
priority_queue_t ** add_initial_values(priority_queue_t ** list_queues, double(** distances), int n, int num_threads, double bestTourCost, queue_element* node_initial);

//Creates results
bestTourPair** init_results(int num_threads, int* tour, double bestTourCost);

//returns pair with the tour path(starting at 0 and ending at 0) and the Cost
bestTourPair *TSPBB(double(** distances), int n, double bestTourCost, int num_threads);

double calculateLB(double ** distances, int n);

void findTwoSmallest(double *edges, int n, double *smallests);

void* updateTour(int (*newTour), int (*tour), int length);

int checkInTour(int (*tour), int city, int length);

double calculateNewLB(double(** distances),queue_element* city_from, int city_to, int length);

void print_matrix(double** distances, int n);

void freeDistances(double **distances, int n);

#endif
