#!/bin/bash
#SBATCH -J bench_saxpy
#SBATCH -N 1
#SBATCH -c 72
#SBATCH --partition grace
#SBATCH --gres=gpu:1
#SBATCH --ntasks-per-node=1
#SBATCH --mail-user=your.email@wherever
#SBATCH --mail-type=END

cd $SLURM_SUBMIT_DIR

module load nvidia/cuda/12.4

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
exe=./saxpy.double
$exe 22
$exe 23
$exe 24
$exe 25
$exe 26
$exe 27
$exe 28
$exe 29
