#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen15-n16.out
#SBATCH --error=gen15-n16.err
#SBATCH --ntasks=16
srun ./tsp-mpi pub-instances/pub-instances/gen15-25.in 25