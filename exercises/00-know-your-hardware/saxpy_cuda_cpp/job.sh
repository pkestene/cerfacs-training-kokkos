#!/bin/bash
#SBATCH -J test_kokkos
#SBATCH -N 1
#SBATCH -c 16
#SBATCH --partition kokkos
#SBATCH --gres=gpu:a30:4
#SBATCH --ntasks-per-node=1
#SBATCH --mail-user=your.email@wherever
#SBATCH --mail-type=END

cd $SLURM_SUBMIT_DIR

module load nvidia/cuda/12.0

# Set OMP_NUM_THREADS to the same value as -c
# with a fallback in case it isn't set.
# SLURM_CPUS_PER_TASK is set to the value of -c, but only if -c is explicitly set
if [ -n "$SLURM_CPUS_PER_TASK" ]; then
  omp_threads=$SLURM_CPUS_PER_TASK
else
  omp_threads=1
fi
export OMP_NUM_THREADS=$omp_threads
export OMP_PROC_BIND=spread
export OMP_PLACES=threads

# run your executable
./saxpy.float 22
./saxpy.float 23
./saxpy.float 24
./saxpy.float 25
./saxpy.float 26
./saxpy.float 27
