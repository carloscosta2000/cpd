#include <stdio.h>
#include "../nqueue/queue.h"
#include <sys/types.h>
#include "tsp.h"

int main(int argc, char *argv[]) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char * n_edges = NULL;
    int n;


    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    
    //reads first line
    if((read = getline(&n_edges, &len, fp)) == -1)
        exit(EXIT_FAILURE);
    
    sscanf(strtok(n_edges, " "), "%d", &n); 
    if (n_edges)
        free(n_edges);
    double** distances = (double **) malloc(sizeof(double) *n);
    for(int i = 0; i < n; i++) 
        distances[i] = (double *)malloc(n * sizeof(double));
    
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

    //print_matrix(distances, n);
    
    fclose(fp);
    if (line)
        free(line);

    double bestTourCost = atof(argv[2]);
    bestTourPair *pair = TSPBB(distances, n, bestTourCost);
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

queue_element *queueElementCreate(int(* tour), double cost, double lb, int length, int city){
    queue_element *newElement;
    newElement = malloc(sizeof(queue_element));
    newElement -> tour = tour;
    newElement -> cost = cost;
    newElement -> lb = lb;
    newElement -> length = length;
    newElement -> city = city; 
    return newElement;
}

bestTourPair *bestTourPairCreate(int *bestTour, double bestTourCost){
    bestTourPair *btPair;
    btPair = malloc(sizeof(bestTourPair));
    btPair -> bestTour = bestTour;
    btPair -> bestTourCost = bestTourCost;
    return btPair;
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

/*char cmp(void* queue_element_1, void* queue_element_2){
    queue_element *e1 = (queue_element*) queue_element_1;
    queue_element *e2 = (queue_element*) queue_element_2;
    if(e1 ->lb < e2 -> lb)
        return 0;
    else
        if(e1 ->lb == e2 -> lb)
            if((e1 -> city == e2 -> city && e1 -> cost < e2 -> cost) || e1 -> city < e2 -> city)
                return 0;
    return 1;

}*/

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

bestTourPair *TSPBB(double(** distances), int n, double bestTourCost){
    int *tour = malloc((n+1)* sizeof(int));
    tour[0] = 0;
    for(int i = 1; i < n+1; i++)
        tour[i] = -1;

    double lb = calculateLB(distances, n);
    printf("%lf\n", lb);
    if(lb > bestTourCost){
        return bestTourPairCreate(tour, -1.0);
    }
    priority_queue_t *queue = queue_create(cmp);
    queue_push(queue, queueElementCreate(tour, 0, lb, 1, 0));
    int* bestTour = malloc((n+1)* sizeof(int));
    updateTour(bestTour, tour, n+1);
    char ze = 0;
    while(queue -> size != 0){
        queue_element *node = (queue_element*) queue_pop(queue);
        // if(node -> tour[node -> length -2] == 11) {
        //     for (int i = 0; i < n; i++) {
        //         printf("%d ", node -> tour[i]);
        //     }
        //     printf("\n%lf", node -> lb);
        //     printf("\n");
        //     if(scanf("%c", &ze) == 0) {
        //         continue;
        //     }

        // }
        if(node -> lb >= bestTourCost)
            return bestTourPairCreate(bestTour, bestTourCost);
        if(node -> length == n && distances[node -> city][0] != 0){
            if(node -> cost + distances[node -> city][0] < bestTourCost){
                insertTour(node->tour, 0, n+1);
                updateTour(bestTour, node->tour, n+1);
                bestTourCost = node -> cost + distances[node -> city][0];
            }
        }else{
            for(int v = 0; v < n; v++){
                if(distances[node->city][v] != 0 && checkInTour(node->tour, v, n+1) == 0){
                    double newLb = calculateNewLB(distances, node, v, n);
                    if(newLb > bestTourCost)
                        continue;
                    int* newTour = malloc((n+1)* sizeof(int));
                    updateTour(newTour, node->tour, n+1);
                    insertTour(newTour, v, n+1);
                    double newCost = distances[node->city][v] + node -> cost;
                    queue_push(queue, queueElementCreate(newTour, newCost, newLb, node->length+1, v));
                    //free(newTour);
                }
            }
        }

    }
    free(bestTour);
    free(tour);
    free(distances);
    return bestTourPairCreate(bestTour, bestTourCost);
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

void insertTour(int (*tour), int city, int length){
    for(int i = 0; i < length; i++)
        if(tour[i] == -1){
            tour[i] = city;
            break; 
        }
}

void* updateTour(int (*newTour), int (*tour), int length){
    for(int i = 0; i < length; i++)
        newTour[i] = tour[i];
    return newTour;
}

int checkInTour(int (*tour), int city, int length){
    for(int i = 0; i < length; i++)
        if(tour[i] == city)
            return 1;
    return 0;
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