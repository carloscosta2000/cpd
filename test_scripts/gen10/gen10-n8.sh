#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen10-n8.out
#SBATCH --error=gen10-n8.err
#SBATCH --ntasks=8
srun ./tsp-mpi pub-instances/pub-instances/gen10-20.in 20