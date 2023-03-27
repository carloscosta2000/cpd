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
    int * path_to_zero;
    double cost;
    double lb;
    int length;
    int city;
} queue_element;

void list_queues_delete(priority_queue_t** queues);

queue_element *queueElementCreate(int *tour, double cost, double lb, int length, int city, int *path_to_zero, int links_to_zero);

void queue_element_delete(queue_element *e);

void bestTourPairDelete(bestTourPair *btPair);

bestTourPair *bestTourPairCreate(int *bestTour, double bestTourCost);

void findTwoSmallest(double *edges, int n, double *smallests);

double calculateLB(double **distances, int n);

void print_queue_node(FILE *fp, void *data);

char cmp(void* queue_element_1, void* queue_element_2);

//Creates queues
priority_queue_t ** init_list_queues(int num_threads);

//Adds 0's neighbours to workers queues.
priority_queue_t ** add_initial_values(priority_queue_t ** list_queues, double(** distances), int n, int num_threads, double bestTourCost, queue_element* node_initial, int * paths_to_zero, int links_to_zero);

int get_biggest_queue_size(priority_queue_t ** list_queues, priority_queue_t *queue);

int get_links_paths_to_zero(double(** distances), int n);

int* fill_paths_to_zero(double(** distances), int n, int links_to_zero);

int check_paths_to_zero(queue_element* element, int links_to_zero);

void print_matrix(double** distances, int n);

void* updateTour(int (*newTour), int (*tour), int length);

int checkInTour(int (*tour), int city, int length);

double calculateNewLB(double(** distances),queue_element* city_from, int city_to, int length);

bestTourPair *TSPBB(double(** distances), int n, double bestTourCost_copy, int num_threads);

#endif
