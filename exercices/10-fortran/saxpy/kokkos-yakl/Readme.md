# SAXPY example using Kokkos + YAKL

This example is adapted from [03-axpy-view](https://github.com/kokkos/kokkos-fortran-interop/tree/develop/examples/03-axpy-view) located in [Kokkos flcl](https://github.com/kokkos/kokkos-fortran-interop) source tree but it does **not** use kokkos-flcl, just plain c++/kokkos, iso-c-bindings and [YAKL](https://github.com/mrnorman/YAKL) for (UVM) memory allocation and data types.

The final code is slightly different from [kokkos-flcl](../kokkos-flcl)

# Build instruction on kraken for OpenMP

```shell
module load compiler/gcc/11.2.0
module load lib/hwloc/2.1.0
module load kokkos/4.1.00-openmp-gnu-11.2.0-RelWithDebInfo
module load yakl/openmp-gnu-11.2.0

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
module load yakl/cuda-12.0-gnu-11.2.0

mkdir _build/cuda
cd _build/cuda
ccmake ../..
make
```

# Activity
