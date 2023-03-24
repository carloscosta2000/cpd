# Define the compiler to use
CC = gcc

# Define the compiler flags to use
CFLAGS = -fopenmp -Wall -g -O3 -fopenmp

# Define the directories for source and object files
SRC_DIR = src
OBJ_DIR = obj
QUEUE_DIR = nqueue

# Create the obj directory
$(shell mkdir -p $(OBJ_DIR))

# Define the source files to compile
SRCS = $(SRC_DIR)/tsp.c $(QUEUE_DIR)/queue.c

# Define the object files to generate
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Define the target executable file
TARGET = tsp-omp

# Define the rule to compile the source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Define the rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Define the default rule to build the target executable
all: $(TARGET)

# Define the rule to clean up object files and the target executable
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) ex2.in 40
