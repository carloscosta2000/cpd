#!/bin/bash
#SBATCH --job-name=cpd
#SBATCH --output=gen10-n2.out
#SBATCH --error=gen10-n2.err
#SBATCH --ntasks=2
srun ./tsp-mpi pub-instances/pub-instances/gen10-20.in 20