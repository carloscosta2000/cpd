#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen20-n1.out
#SBATCH --error=tests/gen20-n1.err
#SBATCH --ntasks=1
srun ./tsp-mpi pub-instances/pub-instances/gen20-5000.in 5000