#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen19-n64.out
#SBATCH --error=tests/gen19-n64.err
#SBATCH --ntasks=64
srun ./tsp-mpi pub-instances/pub-instances/gen19-23.in 23