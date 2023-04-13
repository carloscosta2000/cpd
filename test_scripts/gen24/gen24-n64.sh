#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen24-n64.out
#SBATCH --error=tests/gen24-n64.err
#SBATCH --ntasks=64
srun ./tsp-mpi pub-instances/pub-instances/gen24-50000.in 50000