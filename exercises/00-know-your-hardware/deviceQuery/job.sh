#!/bin/bash
#SBATCH -J cuda_device_query
#SBATCH -N 1
#SBATCH --partition grace
#SBATCH --gres=gpu:1
#SBATCH --ntasks-per-node=1
#SBATCH --mail-user=your.email@wherever
#SBATCH --mail-type=END

cd $SLURM_SUBMIT_DIR

# run your executable
./deviceQuery
