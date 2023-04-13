#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen19-n1.out
#SBATCH --error=gen19-n1.err
#SBATCH --ntasks=1
srun ./tsp-mpi pub-instances/pub-instances/gen19-23.in 23