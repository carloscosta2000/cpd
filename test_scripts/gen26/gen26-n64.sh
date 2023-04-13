#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen26-n64.out
#SBATCH --error=tests/gen26-n64.err
#SBATCH --ntasks=64
srun ./tsp-mpi pub-instances/pub-instances/gen26-50000.in 50000