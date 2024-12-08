# Laplace

Again a stencil computation.

This example is loosely adapted from Nvidia's OpenAcc tutorial:
https://github.com/NVIDIA-OpenACC-Course/nvidia-advanced-openacc-course-sources

## Notes

- Here, for simplifying the problem, we only parallelize computation, but not memory (i.e. every MPI task allocates the entire domain). Parallelization strategy is divide by chunk along y-axis.
- Each MPI task only needs to modify loop indexes `iy_start`, `iy_end` to compute its assigned chunk. So this is not a genuine domain decomposition

## modulefiles environment

We will use nvhpc which provides a cuda-aware MPI implementation.
Please note that by default, nvhpc uses its own compiler, .i.e the module exports environment
variables `CC=nvc` and `CXX=nvc++`.


```shell
module unload nvidia/cuda
module load nvhpc/24.7
```

## Build for CPU (Kokkos::OpenMP) on calypso

```shell
# if using OpenMP as default backend
module load kokkos/4.4.01-openmp-gnu-12.3.0-RelWithDebInfo
mkdir -p _build/openmp
cd _build/openmp
cmake ../..
make
```

## Build for GPU (Kokkos::Cuda) on calypso

```shell
# if using Cuda as default backend
module load kokkos/4.4.01-cuda-12.4-gnu-12.3.0-RelWithDebInfo
mkdir -p _build/cuda
cd _build/cuda
cmake ../..
make
```

## Example run on calypso

Use submit jobs.

# Possible activities

1. implement 2D paralelization (along X and Y axis)
2. change both memory layout and Kokkos::MDRangePolicy template `iterate` parameter for e.g. the OpenMP backend. See how it affects performance.
   more precisely, for Kokkos::OpenMP, default memory layout is RightLayout, and default Kokkos::Iterate is Kokkos::Iterate::Right
