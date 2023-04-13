#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen19-n64.out
#SBATCH --error=gen19-n64.err
#SBATCH --ntasks=64
srun ./tsp-mpi pub-instances/pub-instances/gen19-23.in 23