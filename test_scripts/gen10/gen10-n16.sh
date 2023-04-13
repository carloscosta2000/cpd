#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen10-n16.out
#SBATCH --error=tests/gen10-n16.err
#SBATCH --ntasks=16
srun ./tsp-mpi pub-instances/pub-instances/gen10-20.in 20