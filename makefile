# Define the compiler to use
CC = mpicc

# Define the compiler flags to use
CFLAGS = -Wall -Werror -g -O3 -fopenmp

OBJS = tsp.o queue.o

tsp-mpi: $(OBJS)
	$(CC) $(CFLAGS) -o tsp-mpi $(OBJS) -lm

tsp.o: src/tsp.c src/tsp.h
	$(CC) $(CFLAGS) -c src/tsp.c

queue.o: nqueue/queue.c nqueue/queue.h
	$(CC) $(CFLAGS) -c nqueue/queue.c

clean:
	rm -f tsp-mpi $(OBJS)

