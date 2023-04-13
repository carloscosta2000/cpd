#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen10-n4.out
#SBATCH --error=tests/gen10-n4.err
#SBATCH --ntasks=4
srun ./tsp-mpi pub-instances/pub-instances/gen10-20.in 20