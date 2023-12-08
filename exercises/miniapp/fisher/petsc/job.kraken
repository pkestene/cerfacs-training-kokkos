#!/bin/bash

#SBATCH --job-name="petsc-miniapp-test"
#SBATCH --nodes=1
#SBATCH --partition gpua30
#SBATCH --gres=gpu:a30:4
#SBATCH --output=job%j.out
#SBATCH --time=00:10:00

mpirun ./main -ts_monitor -snes_monitor -ksp_monitor
