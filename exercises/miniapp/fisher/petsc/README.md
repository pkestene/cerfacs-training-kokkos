# TODO

WARNING:

This code is quite old. It probably needs to be refreshed.
TODO: check it still builds and runs.

This is a version of the miniapp which uses PETSc. It is different in spirit from
the other versions of the miniapp, in that it is not intended to be an
essentially-identical piece of code with to-be-ported kernels, but is rather
an example of writing an equivalent code using a higher-level library.

For more information, see the notes in the .c files here

Quick start on kraken (not finished)

1. Load modules
    module list
    module load petsc

2. Build the executable
    make

3. Test in an interactive session
    module load petsc
    make test

   You should see
    Running Test 1
    Success
    Running Test 2
    Success

4. Run your own experiments in the interactive session
    mpirun -n 4 ./main -nx 99 -ny 88 -ts_monitor -snes_monitor -ksp_monitor
    mpirun -n 1 ./main -nx 16 -ny 16 -ts_monitor -snes_monitor -ksp_monitor -assemble 1 -pc_type gamg -dump 1

   You can also modify the included job.kraken and submit with sbatch

   To view the .bov file that is generated (only for single-processor runs with the -dump option), we borrow the procedure from the miniapp
    module load python/x;y.z
    python plotting.py
    display output.png  # make sure that you used ssh -X to log in
