# SAXPY example using Kokkos + FLCL

This example is slightly adapted from [03-axpy-view](https://github.com/kokkos/kokkos-fortran-interop/tree/develop/examples/03-axpy-view) located in [Kokkos flcl](https://github.com/kokkos/kokkos-fortran-interop) source tree.

# Build instruction on kraken for OpenMP

```shell
module load compiler/gcc/11.2.0
module load lib/hwloc/2.1.0
module load kokkos/4.1.00-openmp-gnu-11.2.0-RelWithDebInfo
module load kokkos-flcl/4.1.00-openmp-gnu-11.2.0-RelWithDebInfo

mkdir _build/openmp
cd _build/openmp
ccmake ../..
make
```

# Build instruction on kraken for Cuda

```shell
module load compiler/gcc/11.2.0
module load lib/hwloc/2.1.0
module load kokkos/4.1.00-cuda-12.0-gnu-11.2.0-RelWithDebInfo
module load kokkos-flcl/4.1.00-cuda-12.0-gnu-11.2.0-RelWithDebInfo

mkdir _build/cuda
cd _build/cuda
ccmake ../..
make
```

# Activity

- refactor code to remove array "x", and use only array "x_view" and "x_ptr" everywhere.
