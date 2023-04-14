#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen22-n32.out
#SBATCH --error=gen22-n32.err
#SBATCH --ntasks=32
srun ./tsp-mpi pub-instances/pub-instances/gen22-25000.in 25000