#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen26-n2.out
#SBATCH --error=tests/gen26-n2.err
#SBATCH --ntasks=2
srun ./tsp-mpi pub-instances/pub-instances/gen26-50000.in 50000