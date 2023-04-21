
#include <stdio.h>
#include <sys/types.h>
#include <math.h>
#include <omp.h>
#define N 4
#define M 4
int main() {

    int x[N][M];
    int i,j;
    // #pragma omp parallel private(i,j)
    // for(i = 0; i < N; i++) {
    //     #pragma omp for
    //     for(j = 0; j < M; j++) {
    //         printf("Thread %d a aceder a for loop com os valores i: %d e j: %d\n", omp_get_thread_num(), i, j);
    //         x[i][j] = func(i, j);
    //     }
    // }

    // for(i = 0; i < N; i++) {
    //     #pragma omp parallel for
    //     for(j = 0; j < M; j++) {
    //         printf("Thread %d a aceder a for loop com os valores i: %d e j: %d\n", omp_get_thread_num(), i, j);
    //         x[i][j] = func(i, j);
    //     }
    // }

    int c = 0;
    #pragma omp parallel for reduction(+:c) num_threads(2)
        for(i = 0; i < 4; i++){
            c += i;
            printf("%d\n", c);
        }
        printf("%d\n", c);


}

int func(int i, int j) {
    return i + j;
}