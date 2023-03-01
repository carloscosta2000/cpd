#include <stdio.h>
#include "nqueue/nqueue/queue.h"
#include <sys/types.h>
#include "tsp.h"

int main(int argc, char *argv[]) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char * n_edges = NULL;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    
    //reads first line
    if((read = getline(&n_edges, &len, fp)) != -1){
        printf("Num Cities and Edges: %s", n_edges);
    }
    int n;
    sscanf(strtok(n_edges, " "), "%d", &n); 
    if (n_edges)
        free(n_edges);
    int** distances = (int **) malloc(sizeof(int) *n);
    for(int i = 0; i < n; i++) 
        distances[i] = (int *)malloc(n * sizeof(int));
    
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            distances[i][j] = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        int first_city;
        sscanf(strtok(line, " "), "%d", &first_city);
        int second_city;
        sscanf(strtok(NULL, " "), "%d", &second_city);
        int edge;
        sscanf(strtok(NULL, " "), "%d", &edge);
        distances[first_city][second_city] = edge;
        distances[second_city][first_city] = edge;   
    }
    fclose(fp);
    if (line)
        free(line);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%d ", distances[i][j]);
        }
        printf("\n");
    }
    
    TSPBB(distances, n, 0.0); 
}

bestTourPair *BestTourPairCreate(int n){
    bestTourPair *btPair;
    btPair = malloc(sizeof(bestTourPair));
    btPair -> bestTour = malloc(n * sizeof(int));
    btPair -> bestTourCost = 0;
    return btPair;
}

void findTwoSmallest(int *edges, int n, int *smallests){
    int min1 = 0, min2 = 0;
    for(int i = 0; i < n; i++){
        if(edges[i] != 0){
            if(min1 == 0 || edges[i] < min1){
                min2 = min1;
                min1 = edges[i];
            }
            else if(min2 == 0 || edges[i] < min2){
                min2 = edges[i];
            }
        }
    }
    smallests[0] = min1;
    smallests[1] = min2;
}

double calculateLB(int **distances, int n){
    double lb = 0.0;
    int smallests[2];
    for(int i = 0; i < n; i++){
        findTwoSmallest(distances[i], n, smallests);
        lb += (smallests[0] + smallests[1]);
    }
    return 0.5 * lb;
}

char cmp(void* queue_element_1, void* queue_element_2){
    return NULL;
}

bestTourPair *TSPBB(int(** distances), int n, double bestTourCost){
    int tour[n+1];
    tour[0] = 0;
    for(int i = 1; i < n+1; i++)
        tour[i] = -1;

    double lb = calculateLB(distances, n);
    printf("LB: %f\n", lb);

    priority_queue_t *queue = queue_create(cmp);

    return BestTourPairCreate(n+1);
}