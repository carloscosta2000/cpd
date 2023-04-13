#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen15-n2.out
#SBATCH --error=tests/gen15-n2.err
#SBATCH --ntasks=2
srun ./tsp-mpi pub-instances/pub-instances/gen15-25.in 25