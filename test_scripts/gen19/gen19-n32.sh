#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen19-n32.out
#SBATCH --error=gen19-n32.err
#SBATCH --ntasks=32
srun ./tsp-mpi pub-instances/pub-instances/gen19-23.in 23