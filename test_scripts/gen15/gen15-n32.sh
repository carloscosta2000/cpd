#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen15-n32.out
#SBATCH --error=tests/gen15-n32.err
#SBATCH --ntasks=32
srun ./tsp-mpi pub-instances/pub-instances/gen15-25.in 25