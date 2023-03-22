#include <stdio.h>


int main(int argc, char *argv[]){
    int finished = 0;
    int i = 0;
    while(finished == 0){
        if(i == 5){
            finished = 1;
            continue;
        }
        printf("Olá %d \n", i);
        i++;
    }
}

if(omp_get_thread_num() == 3){
    printf("Node: %d tour -> ", node -> city);
    for(int i = 0; i < node -> length; i++){
        printf("%d ", node -> tour[i]);
    }
    printf("LB: %f LENGTH: %d\n", node -> lb, node ->length);
}