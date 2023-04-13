#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=tests/gen10-n32.out
#SBATCH --error=tests/gen10-n32.err
#SBATCH --ntasks=32
srun ./tsp-mpi pub-instances/pub-instances/gen10-20.in 20