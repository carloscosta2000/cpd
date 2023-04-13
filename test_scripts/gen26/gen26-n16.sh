#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen26-n16.out
#SBATCH --error=gen26-n16.err
#SBATCH --ntasks=16
srun ./tsp-mpi pub-instances/pub-instances/gen26-50000.in 50000