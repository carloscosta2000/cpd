#include <stdio.h>
#include "../nqueue/queue.h"
#include <sys/types.h>
#include "tsp.h"
#include <omp.h>

int main(int argc, char *argv[]) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char * n_edges = NULL;
    int n;
    double exec_time;
    bestTourPair *bestTour;
    //export OMP_NUM_THREADS = 4

    int num_threads = 0;
    #pragma omp parallel 
    {
        num_threads = omp_get_num_threads();
    }
    printf("Threads: %d\n", num_threads);

    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    
    if((read = getline(&n_edges, &len, fp)) == -1)
        exit(EXIT_FAILURE);

    sscanf(strtok(n_edges, " "), "%d", &n); 

    double** distances = (double **) malloc(sizeof(double) *n);
    
    #pragma omp parallel for
    for(int i = 0; i < n; i++) 
        distances[i] = (double *)calloc(n, sizeof(double));
    if(n_edges)
        free(n_edges);

    //TODO: threads para ler um subset de linhas!
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
    exec_time = -omp_get_wtime();
    
    bestTour = TSPBB(distances, n, bestTourCost, num_threads);
    exec_time += omp_get_wtime();
    fprintf(stderr, "%.1fs\n", exec_time);
    //print solution
    if(bestTour -> bestTourCost == -1.0)
        printf("NO SOLUTION\n");
    else{
        printf("%.1f\n", bestTour->bestTourCost);  
        for(int i = 0; i < n+1; i++)
            printf("%d ", bestTour -> bestTour[i]);
        printf("\n");
    }
    for (int i = 0; i < n; i++) {
        free(distances[i]);
    }
    free(distances);
    bestTourPairDelete(bestTour);
    return 0;
}

void list_queues_delete(priority_queue_t** queues){
    for(int i = 0; i < omp_get_num_threads(); i++){
        while (queues[i] -> size != 0) {
            queue_element_delete(queue_pop(queues[i]));
        }
        queue_delete(queues[i]);
        free(queues[i]);
    }
}

queue_element *queueElementCreate(int *tour, double cost, double lb, int length, int city) {
    queue_element *newElement = malloc(sizeof(queue_element));
    if (newElement == NULL) {
        fprintf(stderr, "Error: malloc failed in queueElementCreate\n");
        exit(EXIT_FAILURE);
    }
    newElement->tour = malloc(length * sizeof(int));
    if (newElement->tour == NULL) {
        fprintf(stderr, "Error: malloc failed in queueElementCreate\n");
        free(newElement);
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < length - 1; i++)
        newElement->tour[i] = tour[i];
    newElement->tour[length-1] = city;
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

bestTourPair *bestTourPairCreate(int *bestTour, double bestTourCost){
    bestTourPair *btPair = malloc(sizeof(bestTourPair));
    btPair->bestTour = bestTour;
    btPair -> bestTourCost = bestTourCost;
    return btPair;
}

void findTwoSmallest(double *edges, int n, double *smallests){
    double min1 = 0.0, min2 = 0.0;
    //#pragma omp parallel for reduction(min:min1) reduction(min:min2)
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
    #pragma omp parallel for reduction(+:lb)
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
    fprintf(fp, "City: %d\n\n", node->city);
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

bestTourPair *TSPBB(double(** distances), int n, double bestTourCost, int num_threads){
    int *tour = (int*) calloc((n+1), sizeof(int));
    double lb = calculateLB(distances, n);
    if(lb > bestTourCost){ //caso nao tenha solução
        return bestTourPairCreate(tour, -1.0);
    }
    priority_queue_t ** list_queues = (priority_queue_t**) malloc(sizeof(priority_queue_t) * (num_threads));
    //Creates queues
    for(int i = 0; i < num_threads; i++){
        list_queues[i] = queue_create(cmp);
    }
    //Adds 0's neighbours to workers queues.
    queue_element* node_initial = queueElementCreate(tour, 0, lb, 1, 0);
    int current_index = 0;
    for(int i = 0; i < n; i++){
        if(distances[0][i] != 0){
            double newLb = calculateNewLB(distances, node_initial, i, n);
            if(newLb > bestTourCost){
                printf("não entra\n");
                continue;
            }
            double newCost = distances[node_initial->city][i] + node_initial->cost;      
            queue_push(list_queues[current_index % (num_threads)], queueElementCreate(node_initial->tour, newCost, newLb, 2, i));
            current_index++;
        }
    }
    bestTourPair** results = (bestTourPair**) malloc((num_threads) * sizeof(bestTourPair));
    for(int i = 0; i < num_threads; i++){
        results[i] = bestTourPairCreate(tour, bestTourCost);
    }
    // FILE * fp = fopen("out", "w");
    // for(int i = 0; i < num_threads; i++){
    //     printf("Queue %d: \n", i);
    //     priority_queue_t *queue_copy = queue_duplicate(list_queues[i]);
    //     printf("Size: %ld\n", queue_copy -> size);
    //     while (queue_copy->size > 0){
    //         void* node = queue_pop(queue_copy);
    //         fprintf(fp, "Queue: %d\n", i);
    //         print_queue_node(fp, node);
    //     }
    //     queue_delete(queue_copy);
    //     free(queue_copy);
    // }
    #pragma omp parallel
    {   
        double bestTourCost_threads = bestTourCost;
        int finished = 0;
        priority_queue_t *queue = list_queues[omp_get_thread_num()];
        int* bestTour = (int*) calloc((n+1), sizeof(int));
        if(queue -> size != 0){
            priority_queue_t *queue_duplicated = queue_duplicate(queue);
            queue_element * current = (queue_element*) queue_pop(queue_duplicated);  
            updateTour(bestTour, current->tour, current->length);
            bestTourCost_threads = current -> cost;
        }
        double newLb;
        while(queue -> size != 0 && finished == 0){
            queue_element *node = (queue_element*) queue_pop(queue);
            if(node -> lb >= bestTourCost){
                finished = 1;
                results[omp_get_thread_num()] = bestTourPairCreate(bestTour, bestTourCost_threads);
            }  
            if(node -> length == n && distances[node -> city][0] != 0){
                if(node -> cost + distances[node -> city][0] < bestTourCost){
                    updateTour(bestTour, node->tour, node->length);
                    bestTour[n] = 0;
                    bestTourCost_threads = node -> cost + distances[node -> city][0];
                }
            }else{
                for(int v = 0; v < n; v++){
                    if(distances[node->city][v] != 0 && checkInTour(node->tour, v, node -> length) == 0){
                        newLb = calculateNewLB(distances, node, v, n);
                        if(newLb > bestTourCost){
                            continue;
                        }  
                        double newCost = distances[node->city][v] + node -> cost;
                        queue_push(queue, queueElementCreate(node->tour, newCost, newLb, node->length+1, v));
                    }
                }
            }
            queue_element_delete(node);
        }
        results[omp_get_thread_num()] = bestTourPairCreate(bestTour, bestTourCost_threads);
        // while (queue -> size != 0) {
        //     queue_element_delete(queue_pop(queue));
        // }
        //queue_delete(queue);
        // free(queue);
    }
    free(tour);
    list_queues_delete(list_queues);
    int best = 0, cost_min = results[0] -> bestTourCost;
    int bestSameCostCounter = 0;

    for(int i = 0; i < num_threads; i++){
        printf("BestTourCost : %f\n", results[i]-> bestTourCost);
        if(results[i]-> bestTourCost <= cost_min){
            best = i;
            cost_min = results[i]->bestTourCost;
            bestSameCostCounter++;
        }
    }
    
    return results[best];
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

void* updateTour(int (*newTour), int (*tour), int length){
    #pragma omp parallel for
    for(int i = 0; i < length; i++)
        newTour[i] = tour[i];
    return newTour;
}

int checkInTour(int (*tour), int city, int length){
    for(int i = 0; i < length; i++){
        if(tour[i] == city){
            return 1;
        }
    }
    return 0;
} //We have tested and its not worth it to parallelize

/*int checkInTour(int (*tour), int city, int length){
    int found = 0;
    #pragma omp parallel for reduction(+:found)
    for(int i = 0; i < length; i++){
        if(tour[i] == city){
            found = 1;
            break;
        }
    }
    return found;
}*/

/*int checkInTour(int (*tour), int city, int length){
    int found = 0;
    #pragma omp parallel for shared(found)
    for(int i = 0; i < length; i++){
        if(tour[i] == city){
            #pragma omp critical
            found = 1;
        }
    }
    return found;
}*/

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