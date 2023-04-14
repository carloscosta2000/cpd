#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen10-n64.out
#SBATCH --error=gen10-n64.err
#SBATCH --ntasks=64
srun ./tsp-mpi pub-instances/pub-instances/gen10-20.in 20