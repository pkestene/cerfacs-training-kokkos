# SAXPY example using Kokkos + FLCL

This example is slightly adapted from [03-axpy-view](https://github.com/kokkos/kokkos-fortran-interop/tree/develop/examples/03-axpy-view) located in [Kokkos flcl](https://github.com/kokkos/kokkos-fortran-interop) source tree.

# Build instruction on hpcai for OpenMP

```shell
module load kokkos/4.1.00-openmp-gnu-9.4.0-RelWithDebInfo
module load kokkos-flcl/4.1.00-openmp-gnu-9.4.0-RelWithDebInfo

mkdir _build/openmp
cd _build/openmp
ccmake ../..
make
```

# Build instruction on hpcai for Cuda

```shell
module load kokkos/4.1.00-cuda-11.8-gnu-9.4.0-RelWithDebInfo
module load kokkos-flcl/4.1.00-cuda-11.8-gnu-9.4.0-RelWithDebInfo

mkdir _build/cuda
cd _build/cuda
ccmake ../..
make
```

# Activity

- refactor code to remove array "x", and use only array "x_view" and "x_ptr" everywhere.
