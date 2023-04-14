#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen20-n16.out
#SBATCH --error=gen20-n16.err
#SBATCH --ntasks=16
srun ./tsp-mpi pub-instances/pub-instances/gen20-5000.in 5000