#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen19-n16.out
#SBATCH --error=gen19-n16.err
#SBATCH --ntasks=16
srun ./tsp-mpi pub-instances/pub-instances/gen19-23.in 23