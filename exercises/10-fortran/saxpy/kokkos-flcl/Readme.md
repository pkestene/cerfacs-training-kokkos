# SAXPY example using Kokkos + FLCL

This example is slightly adapted from [03-axpy-view](https://github.com/kokkos/kokkos-fortran-interop/tree/develop/examples/03-axpy-view) located in [Kokkos flcl](https://github.com/kokkos/kokkos-fortran-interop) source tree.

# Build instruction on calypso for OpenMP

```shell
module load gcc/12.3.0_arm
module load lib/hwloc/2.11.2
module load kokkos/4.4.01-openmp-gnu-12.3.0-RelWithDebInfo
module load kokkos-flcl/4.4.01-openmp-gnu-12.3.0-RelWithDebInfo

mkdir _build/openmp
cd _build/openmp
ccmake ../..
make
```

# Build instruction on calypso for Cuda

```shell
module load gcc/12.3.0_arm
module load lib/hwloc/2.11.2
module load kokkos/4.4.01-cuda-12.4-gnu-12.3.0-RelWithDebInfo
module load kokkos-flcl/4.4.01-cuda-12.4-gnu-12.3.0-RelWithDebInfo

mkdir _build/cuda
cd _build/cuda
ccmake ../..
make
```

# Activity

- refactor code to remove array "x", and use only array "x_view" and "x_ptr" everywhere.
