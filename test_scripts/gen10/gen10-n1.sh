#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen10-n1.out
#SBATCH --error=gen10-n1.err
#SBATCH --ntasks=1
srun ./tsp-mpi pub-instances/pub-instances/gen10-20.in 20