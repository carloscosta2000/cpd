#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen19-n2.out
#SBATCH --error=tests/gen19-n2.err
#SBATCH --ntasks=2
srun ./tsp-mpi pub-instances/pub-instances/gen19-23.in 23