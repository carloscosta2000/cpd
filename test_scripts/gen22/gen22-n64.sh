#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen22-n64.out
#SBATCH --error=tests/gen22-n64.err
#SBATCH --ntasks=64
srun ./tsp-mpi pub-instances/pub-instances/gen22-25000.in 25000