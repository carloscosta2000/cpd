#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen22-n4.out
#SBATCH --error=gen22-n4.err
#SBATCH --ntasks=4
srun ./tsp-mpi pub-instances/pub-instances/gen22-25000.in 25000