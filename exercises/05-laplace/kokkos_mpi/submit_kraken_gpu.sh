#!/bin/bash
#SBATCH -J test_mpi_kokkos_gpu                       # Job name
#SBATCH -N 2                                         # number of nodes
#SBATCH -n 8                                         # total number of MPI task
#SBATCH -c 8                                         # number of CPU cores per task
#SBATCH -o test_mpi_kokkos_gpu.%j.out                # stdout filename
#SBATCH --partition gpua30                           # queue name
#SBATCH --gres=gpu:a30:4

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

cd $SLURM_SUBMIT_DIR

# just for cross-checking GPU/task affinity done by slurm
echo "CUDA_VISIBLE_DEVICES = $CUDA_VISIBLE_DEVICES"

EXE_NAME=laplace_kokkos

# Default behaviour : all mpi tasks are binded to the same GPU
mpirun --report-bindings ./$EXE_NAME

echo " "
echo "##############################################"
echo "##############################################"
echo "##############################################"
echo " "

# Nominal bahavior: each mpi task binded to a different GPU
mpirun --report-bindings ./$EXE_NAME --kokkos-map-device-id-by=mpi_rank
