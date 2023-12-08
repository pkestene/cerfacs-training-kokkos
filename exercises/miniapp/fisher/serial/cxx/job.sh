#!/bin/bash
#SBATCH -J fisher_serial
#SBATCH -N 1
#SBATCH -c 32
#SBATCH --partition gpua30
#SBATCH --gres=gpu:a30:4
#SBATCH --ntasks-per-node=1

cd $SLURM_SUBMIT_DIR

# now run on a 128x128 grid to t=0.01 via 100 time steps
srun ./main 128 128 100 0.01
