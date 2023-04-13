#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen30-n64.out
#SBATCH --error=gen30-n64.err
#SBATCH --ntasks=64
srun ./tsp-mpi pub-instances/pub-instances/gen30-5000.in 5000