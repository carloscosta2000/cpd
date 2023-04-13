#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen26-n1.out
#SBATCH --error=tests/gen26-n1.err
#SBATCH --ntasks=1
srun ./tsp-mpi pub-instances/pub-instances/gen26-50000.in 50000