# Parallel dot product

The exercise is to parallelize a dot product using a league of teams of threads (aka hierarchical or nested parallelism).

# OpenMP target

Have a look at how this is done in regular OpenMP, e.g. :
https://github.com/OpenMP/Examples/blob/v4.5.0/sources/Example_teams.1.c

## Activities

- Go into subfolder `openmp_target`, we have slightly modify this code to add a main and a Makefile.
- check that it build and run using Nvidia nvhpc toolkit. Use `module load nvhpc/24.7` on calypso

# Kokkos

## Activities

1. Preliminary: build and run example `01_thread_teams_lambda` (located inside kokkos sources), to get familiar with nested `parallel_for` and kokkos hierarchical parallelism model

2. Transform the above OpenMP target example into Kokkos code, using template code in folder `kokkos_dot_prod`

3. Build and run the benchmark inside folder `benchmark`.
- change data size,
- change kokkos backend: Kokkos::OpenMP and Kokkos::Cuda,
- compare performance with third party blas library,
- comments ?


Additionnal documentation:
https://kokkos.github.io/kokkos-core-wiki/ProgrammingGuide/HierarchicalParallelism.html?highlight=hierarchical
