#!/bin/bash
#SBATCH -J test_kokkos
#SBATCH -N 1
#SBATCH -c 72
#SBATCH --partition grace
#SBATCH --gres=gpu:1
#SBATCH --ntasks-per-node=1
#SBATCH --mail-user=your.email@wherever
#SBATCH --mail-type=END

cd $SLURM_SUBMIT_DIR

# run your executable
./Kokkos_query_device
