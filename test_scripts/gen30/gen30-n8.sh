#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen30-n8.out
#SBATCH --error=gen30-n8.err
#SBATCH --ntasks=8
srun ./tsp-mpi pub-instances/pub-instances/gen30-5000.in 5000