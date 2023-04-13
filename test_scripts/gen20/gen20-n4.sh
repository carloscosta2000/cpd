#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen20-n4.out
#SBATCH --error=tests/gen20-n4.err
#SBATCH --ntasks=4
srun ./tsp-mpi pub-instances/pub-instances/gen20-5000.in 5000