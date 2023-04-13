#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen24-n16.out
#SBATCH --error=tests/gen24-n16.err
#SBATCH --ntasks=16
srun ./tsp-mpi pub-instances/pub-instances/gen24-50000.in 50000