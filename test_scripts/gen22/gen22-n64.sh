#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen22-n64.out
#SBATCH --error=gen22-n64.err
#SBATCH --ntasks=64
srun ./tsp-mpi pub-instances/pub-instances/gen22-25000.in 25000