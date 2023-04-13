#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen19-n1.out
#SBATCH --error=tests/gen19-n1.err
#SBATCH --ntasks=1
srun ./tsp-mpi pub-instances/pub-instances/gen19-23.in 23