#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen24-n8.out
#SBATCH --error=tests/gen24-n8.err
#SBATCH --ntasks=8
srun ./tsp-mpi pub-instances/pub-instances/gen24-50000.in 50000