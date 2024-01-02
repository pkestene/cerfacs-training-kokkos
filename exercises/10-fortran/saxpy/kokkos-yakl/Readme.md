# SAXPY example using Kokkos + YAKL

This example is adapted from [03-axpy-view](https://github.com/kokkos/kokkos-fortran-interop/tree/develop/examples/03-axpy-view) located in [Kokkos flcl](https://github.com/kokkos/kokkos-fortran-interop) source tree but it does **not** use kokkos-flcl, just plain c++/kokkos, iso-c-bindings and [YAKL](https://github.com/mrnorman/YAKL) for (UVM) memory allocation and data types.

The final code is slightly different from [kokkos-flcl](../kokkos-flcl)

# Build instruction on hpcai for OpenMP

```shell
module load kokkos/4.1.00-openmp-gnu-9.4.0-RelWithDebInfo
module load yakl/openmp-gnu-9.4.0

mkdir _build/openmp
cd _build/openmp
ccmake ../..
make
```

# Build instruction on hpcai for Cuda

```shell
module load kokkos/4.1.00-cuda-11.8-gnu-9.4.0-RelWithDebInfo
module load yakl/cuda-11.8-gnu-9.4.0

mkdir _build/cuda
cd _build/cuda
ccmake ../..
make
```

# Activity
