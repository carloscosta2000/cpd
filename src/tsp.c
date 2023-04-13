#include <stdio.h>
#include "../nqueue/queue.h"
#include <sys/types.h>
#include "tsp.h"
#include <math.h>
#include <omp.h>
#include <mpi.h>
#define TAG 123
#define TAG_BTC 111
#define N 20000

int main(int argc, char *argv[]) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char * n_edges = NULL;
    int n;
    double exec_time;

    int id, p;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);


    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    if((read = getline(&n_edges, &len, fp)) == -1)
        exit(EXIT_FAILURE);

    sscanf(strtok(n_edges, " "), "%d", &n);

    double** distances = (double **) malloc(sizeof(double) *n);
    for(int i = 0; i < n; i++)
        distances[i] = (double *)malloc(n * sizeof(double));
    if(n_edges)
        free(n_edges);

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            distances[i][j] = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        int first_city;
        sscanf(strtok(line, " "), "%d", &first_city);
        int second_city;
        sscanf(strtok(NULL, " "), "%d", &second_city);
        double edge;
        sscanf(strtok(NULL, " "), "%lf", &edge);
        distances[first_city][second_city] = edge;
        distances[second_city][first_city] = edge;
    }

    if(line)
        free(line);
    fclose(fp);

    double bestTourCost = atof(argv[2]);
    int counter = 0;
    exec_time = -omp_get_wtime();

    bestTourPair *pair = TSPBB(distances, n, bestTourCost, id, p, counter);
    if (id == 0) {
        exec_time += omp_get_wtime();
        fprintf(stderr, "%.1fs\n", exec_time);
        //print solution
        if(pair -> bestTourCost == -1.0)
            printf("NO SOLUTION\n");
        else{
            printf("%.1f\n", pair->bestTourCost);
            for(int i = 0; i < n+1; i++)
                printf("%d ", pair -> bestTour[i]);
            printf("\n");
        }

    }
    for (int i = 0; i < n; i++) {
        free(distances[i]);
    }
    free(distances);
    bestTourPairDelete(pair);
    MPI_Finalize();
    return 0;
}

queue_element *queueElementCreate(int *tour, double cost, double lb, int length, int city, long path_zero, long in_tour, int n) {
    queue_element *newElement = malloc(sizeof(queue_element));
    newElement->tour = malloc(n * sizeof(int));
    size_t size = n * sizeof(int);
    memcpy(newElement->tour, tour, size);
    newElement->tour[length-1] = city;

    newElement->path_zero = path_zero & ~(1L << city);
    newElement->in_tour = in_tour |= (1L << city);

    newElement->cost = cost;
    newElement->lb = lb;
    newElement->length = length;
    newElement->city = city;
    return newElement;
}

void queue_element_delete(queue_element *e){
    free(e-> tour);
    free(e);
}

void bestTourPairDelete(bestTourPair *btPair){
    free(btPair->bestTour);
    free(btPair);
}

long fill_paths_to_zero(double(** distances), int n){
    long bit_array = 0;
    for(int i = 1; i < n; i++)
        if(distances[0][i] != 0)
            bit_array |= (1L << i);
    return bit_array;
}

bestTourPair *bestTourPairCreate(int *bestTour, double bestTourCost){
    bestTourPair *btPair = malloc(sizeof(bestTourPair));
    btPair->bestTour = bestTour;
    btPair -> bestTourCost = bestTourCost;
    return btPair;
}

void updateTour(int (*newTour), int (*tour), int length){
    memcpy(newTour, tour, length* sizeof(int));
}

int checkInTour(long in_tour, int city){
    if (in_tour & (1L << city))
        return 1;
    return 0;
}

void findTwoSmallest(double *edges, int n, double *smallests){
    double min1 = 0.0, min2 = 0.0;
    for(int i = 0; i < n; i++){
        if(edges[i] != 0){
            if(min1 == 0.0 || edges[i] < min1){
                min2 = min1;
                min1 = edges[i];
            }
            else if(min2 == 0.0 || edges[i] < min2){
                min2 = edges[i];
            }
        }
    }
    smallests[0] = min1;
    smallests[1] = min2;
}

double calculateLB(double **distances, int n){
    double lb = 0.0;
    double smallests[2];
    for(int i = 0; i < n; i++){
        findTwoSmallest(distances[i], n, smallests);
        lb += (smallests[0] + smallests[1]);
    }
    return 0.5 * lb;
}

void print_queue_node(FILE *fp, void *data) {
    queue_element *node = (queue_element*) data;
    fputs("Tour: ", fp);
    for (int i = 0; i < node->length; i++) {
        fprintf(fp, "%d ", node->tour[i]);
    }
    fprintf(fp, "\n");
    fprintf(fp, "Cost: %.2f\n", node->cost);
    fprintf(fp, "Lower Bound: %.2f\n", node->lb);
    fprintf(fp, "Length: %d\n", node->length);
    fprintf(fp, "City: %d\n", node->city);
}

char cmp(void* queue_element_1, void* queue_element_2) {
    queue_element *e1 = (queue_element*) queue_element_1;
    queue_element *e2 = (queue_element*) queue_element_2;
    if(e1 -> lb < e2 -> lb)
        return 0;
    else if (e1 -> lb > e2 -> lb) {
        return 1;
    } else if (e1 -> city > e2 -> city){
        return 1;
    } else {
        return 0;
    }
}

void print_matrix(double** distances, int n) {
    printf("\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++)
            printf("%.1lf ", distances[i][j]);
        printf("\n");
    }
    printf("\n");
}

double calculateNewLB(double(** distances),queue_element* city_from, int city_to, int length){
    double newLb = 0.0;
    double distance = distances[city_from->city][city_to];
    newLb = (city_from -> lb) + distance;
    double smallests[2];
    findTwoSmallest(distances[city_from->city], length, smallests);
    double min_f = 0;
    if(distance>= smallests[1])
        min_f = smallests[1];
    else
        min_f = smallests[0];
    findTwoSmallest(distances[city_to], length, smallests);
    double min_t = 0;
    if(distance>= smallests[1])
        min_t = smallests[1];
    else
        min_t = smallests[0];
    newLb -= ((min_f + min_t) / 2);
    return newLb;
}

priority_queue_t * scatter(priority_queue_t *queue, int id, int p) {
    priority_queue_t* newQueue = queue_create(cmp);
    int counter = 0;
    while (queue -> size != 0) {
        if (counter % p == id) {
            queue_push(newQueue, queue_pop(queue));
        } else {
            queue_pop(queue);
        }
        counter++;
    }
    return newQueue;
}

priority_queue_t * scatter_to_threads(priority_queue_t *queue, int thread_num) {
    priority_queue_t* new_queue = queue_create(cmp);
    int counter = 0;
    while(queue -> size > 0) {
        if(counter % omp_get_num_threads()) {
            queue_push(new_queue, queue_pop(queue));
        } else {
            queue_pop(queue);
        }
        counter++;
    }
    return new_queue;
}

//Creates queues
// priority_queue_t ** init_list_queues(int num_threads){
//     priority_queue_t ** list_queues = (priority_queue_t**) malloc(sizeof(priority_queue_t) * (num_threads));
//     for(int i = 0; i < num_threads; i++){
//         list_queues[i] = queue_create(cmp);
//     }
//     return list_queues;
// }

double recalculatePathCost(int * tour, double** distances, int n) {
    double tourCost = 0.0;
    for (int i = 0; i < n; i++) {
        tourCost += distances[tour[i]][tour[i+1]];
    }
    return tourCost;
}

bestTourPair *TSPBB(double(** distances), int n, double bestTourCost, int id, int p, int counter){
    int *tour = (int*) calloc((n+1), sizeof(int));
    double lb = calculateLB(distances, n);
    int* bestTour = (int*) calloc((n+1), sizeof(int));
    if(lb > bestTourCost){ //caso nao tenha solução
        return bestTourPairCreate(tour, -1.0);
    }
    priority_queue_t *equal_queue = queue_create(cmp);
    //correr algumas vezes e meter numa queue auxiliar
    long bit_array = 0;
    queue_push(equal_queue, queueElementCreate(tour, 0, lb, 1, 0, fill_paths_to_zero(distances, n), bit_array, n+1));
    double newLb = 0.0;

    int iteration_counter = 0;
    int updateBestTourCost = 0;
    //fill queue up
    while(iteration_counter < p * N && equal_queue  -> size >= 0) {
        //RECIEVE TOUR COST
        if (updateBestTourCost % (N/8) == 0) {
            MPI_Request request;
            double received_best_tour_cost;
            int flag = 0;
            MPI_Irecv(&received_best_tour_cost, 1, MPI_DOUBLE, MPI_ANY_SOURCE, TAG_BTC, MPI_COMM_WORLD, &request);
            MPI_Test(&request, &flag, NULL);
            if (flag && received_best_tour_cost != 0.0 && received_best_tour_cost < bestTourCost) {
                bestTourCost = received_best_tour_cost;
            }
        }
        queue_element *node = (queue_element*) queue_pop(equal_queue);
        if(node -> lb >= bestTourCost){
            break;
        }
        if(node -> length == n && distances[node -> city][0] != 0){
            if(node -> cost + distances[node -> city][0] < bestTourCost){
                updateTour(bestTour, node->tour, n+1);
                bestTourCost = node -> cost + distances[node -> city][0];
            }
        }else{
            if(node -> path_zero == 0){
                queue_element_delete(node);
                continue;
            }
            for(int v = 0; v < n; v++){
                if(distances[node->city][v] != 0 && checkInTour(node->in_tour, v) == 0){
                    newLb = calculateNewLB(distances, node, v, n);
                    if(newLb > bestTourCost)
                        continue;
                    double newCost = distances[node->city][v] + node -> cost;
                    queue_push(equal_queue, queueElementCreate(node->tour, newCost, newLb, node->length+1, v, node -> path_zero, node->in_tour, n+1));
                }
            }
        }
        //SEND TOUR COST
        if (updateBestTourCost % (N/8) == 0) {
            for (int i = 0; i < p; i++) {
                if (i != id) {
                    MPI_Request request;
                    MPI_Isend(&bestTourCost, 1, MPI_DOUBLE, i, TAG_BTC, MPI_COMM_WORLD, &request);
                }
            }
        }
        queue_element_delete(node);
        iteration_counter++;
        updateBestTourCost++;
    }

    priority_queue_t* individual_queue = scatter(equal_queue, id, p);

    //priority_queue_t ** buffers = init_list_queues(omp_get_num_threads);

    updateBestTourCost = 0;
    //Checks individual nodes
    #pragma omp parallel 
    {
        individual_queue = scatter_to_threads(individual_queue, omp_get_thread_num());
        while(individual_queue -> size != 0){
            queue_element *node = (queue_element*) queue_pop(individual_queue);
            //RECIEVE TOUR COST
            if (updateBestTourCost % (N/8) == 0) {
                MPI_Request request;
                double received_best_tour_cost;
                int flag = 0;
                MPI_Irecv(&received_best_tour_cost, 1, MPI_DOUBLE, MPI_ANY_SOURCE, TAG_BTC, MPI_COMM_WORLD, &request);
                MPI_Test(&request, &flag, NULL);
                if (flag && received_best_tour_cost != 0.0 && received_best_tour_cost < bestTourCost) {
                    bestTourCost = received_best_tour_cost;
                }
            }
            if(node -> lb >= bestTourCost){
                break;
            }
            if(node -> length == n && distances[node -> city][0] != 0){
                if(node -> cost + distances[node -> city][0] < bestTourCost){
                    updateTour(bestTour, node->tour, n+1);
                    bestTourCost = node -> cost + distances[node -> city][0];
                }
            }else{
                if(node -> path_zero == 0){
                    queue_element_delete(node);
                    continue;
                }
                for(int v = 0; v < n; v++){
                    if(distances[node->city][v] != 0 && checkInTour(node->in_tour, v) == 0){
                        newLb = calculateNewLB(distances, node, v, n);
                        if(newLb > bestTourCost)
                            continue;
                        double newCost = distances[node->city][v] + node -> cost;
                        queue_push(individual_queue, queueElementCreate(node->tour, newCost, newLb, node->length+1, v, node -> path_zero, node->in_tour, n+1));
                    }
                }
            }
            //SEND TOUR COST
            if (updateBestTourCost % (N/8) == 0) {
                //printf("IN IF\n");
                for (int i = 0; i < p; i++) {
                    if (i != id) {
                        MPI_Request request;
                        MPI_Isend(&bestTourCost, 1, MPI_DOUBLE, i, TAG_BTC, MPI_COMM_WORLD, &request);
                    }
                }
            }
            updateBestTourCost++;
            queue_element_delete(node);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (id == 0) {
        bestTourPair* results = malloc(p * sizeof(bestTourPair));
        if (recalculatePathCost(bestTour, distances, n) != 0.0) {
            results[id] = *bestTourPairCreate(bestTour, recalculatePathCost(bestTour, distances, n));
        } else {
            results[id] = *bestTourPairCreate(bestTour, INFINITY);
        }

        for (int i = 1; i < p; i++) {
            MPI_Status status;
            int* tourAux = malloc((n+1) * sizeof(int));
            MPI_Recv(tourAux, n + 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &status);
            if (recalculatePathCost(tourAux, distances, n) != 0) {
                results[i] = *bestTourPairCreate(tourAux, recalculatePathCost(tourAux, distances, n));
            } else {
                results[i] = *bestTourPairCreate(tourAux, INFINITY);
            }
        }
        double reference = INFINITY;
        for (int i = 0; i < p; i++) {
            if (results[i].bestTourCost < reference) {
                reference = results[i].bestTourCost;
                bestTourCost = results[i].bestTourCost;
                memcpy(bestTour, results[i].bestTour , (n+1) * sizeof(int));
            }
        }
        //free(tour);
        //queue_delete(individual_queue);
        //free(individual_queue);
        return bestTourPairCreate(bestTour, bestTourCost);
    } else {
        MPI_Send(bestTour, n + 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
    }
    free(tour);
    queue_delete(individual_queue);
    free(individual_queue);
    return bestTourPairCreate(NULL, -1.0);
}