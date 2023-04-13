#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen15-n64.out
#SBATCH --error=tests/gen15-n64.err
#SBATCH --ntasks=64
srun ./tsp-mpi pub-instances/pub-instances/gen15-25.in 25