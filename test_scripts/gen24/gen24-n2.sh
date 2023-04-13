#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen24-n2.out
#SBATCH --error=tests/gen24-n2.err
#SBATCH --ntasks=2
srun ./tsp-mpi pub-instances/pub-instances/gen24-50000.in 50000