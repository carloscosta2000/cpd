#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen24-n1.out
#SBATCH --error=gen24-n1.err
#SBATCH --ntasks=1
srun ./tsp-mpi pub-instances/pub-instances/gen24-50000.in 50000