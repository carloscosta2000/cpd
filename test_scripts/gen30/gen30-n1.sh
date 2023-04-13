#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen30-n1.out
#SBATCH --error=gen30-n1.err
#SBATCH --ntasks=1
srun ./tsp-mpi pub-instances/pub-instances/gen30-5000.in 5000