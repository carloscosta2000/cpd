#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen15-n4.out
#SBATCH --error=gen15-n4.err
#SBATCH --ntasks=4
srun ./tsp-mpi pub-instances/pub-instances/gen15-25.in 25