#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen30-n16.out
#SBATCH --error=gen30-n16.err
#SBATCH --ntasks=16
srun ./tsp-mpi pub-instances/pub-instances/gen30-5000.in 5000