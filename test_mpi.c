#include <omp.h>
#include <mpi.h>
#include <stdio.h>
#include<unistd.h>
#define TAG 123
#define TAG_BTC 111
#define N 6
int main(int argc, char *argv[]) {

    int rank, p;
    int a[N];
    int b[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    if (rank == 0) {
         for (int i = 0; i < N; i++) {
            a[i] = i;

         }

    }

    printf("Before line 5\n");
    printf("A\n");
    for (int asd = 0; asd < N; asd++) {
        printf("%d ", a[asd]);
    }
    printf("\n");
    printf("b\n");
    for (int asd = 0; asd < N; asd++) {
        printf("%d ", b[asd]);
    }
    printf("\n");

    MPI_Bcast(a, N, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Before line 6\n");
    printf("A\n");
    for (int asd = 0; asd < N; asd++) {
        printf("%d ", a[asd]);
    }

    printf("\n");
    printf("b\n");
    for (int asd = 0; asd < N; asd++) {
        printf("%d ", b[asd]);
    }
    printf("\n");
    for (int i = 0; i < N; i++) {
        b[i] = a[i] + rank;

    }
    
    
    

    printf("Before line 8\n");
    printf("A\n");
    for (int asd = 0; asd < N; asd++) {
        printf("%d ", a[asd]);
    }
    printf("\n");
    printf("b\n");
    for (int asd = 0; asd < N; asd++) {
        printf("%d ", b[asd]);
    }
    printf("\n");
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Alltoall(&b, N, MPI_INT, a, N, MPI_INT, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    //sleep(5);
    if (rank == 0) {
        printf("After line 8\n");
        for (int asd = 0; asd < N; asd++) {
            printf("a: %d_ ", a[asd]);
        }
        for (int asd = 0; asd < N; asd++) {
            printf("b: %d_ ", b[asd]);
        }

    } else {
        printf("After line 8\n");
        for (int asd = 0; asd < N; asd++) {
            printf("a: %d* ", a[asd]);
        }
        for (int asd = 0; asd < N; asd++) {
            printf("b: %d* ", b[asd]);
        }
    }

    printf("\n");
    MPI_Finalize();
}