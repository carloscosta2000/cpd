#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen19-n4.out
#SBATCH --error=gen19-n4.err
#SBATCH --ntasks=4
srun ./tsp-mpi pub-instances/pub-instances/gen19-23.in 23