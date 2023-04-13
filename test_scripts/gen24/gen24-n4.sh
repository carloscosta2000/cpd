#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen24-n4.out
#SBATCH --error=gen24-n4.err
#SBATCH --ntasks=4
srun ./tsp-mpi pub-instances/pub-instances/gen24-50000.in 50000