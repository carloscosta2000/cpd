#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen19-n8.out
#SBATCH --error=tests/gen19-n8.err
#SBATCH --ntasks=8
srun ./tsp-mpi pub-instances/pub-instances/gen19-23.in 23