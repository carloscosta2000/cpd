#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen15-n8.out
#SBATCH --error=gen15-n8.err
#SBATCH --ntasks=8
srun ./tsp-mpi pub-instances/pub-instances/gen15-25.in 25