#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen15-n1.out
#SBATCH --error=tests/gen15-n1.err
#SBATCH --ntasks=1
srun ./tsp-mpi pub-instances/pub-instances/gen15-25.in 25