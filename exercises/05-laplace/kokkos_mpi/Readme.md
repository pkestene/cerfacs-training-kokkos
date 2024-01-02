# Laplace

Again a stencil computation.

This example is loosely adapted from Nvidia's OpenAcc tutorial:
https://github.com/NVIDIA-OpenACC-Course/nvidia-advanced-openacc-course-sources

## Notes

- Here, for simplifying the problem, we only parallelize computation, but not memory (i.e. every MPI task allocates the entire domain). Parallelization strategy is divide by chunk along y-axis.
- Each MPI task only needs to modify loop indexes `iy_start`, `iy_end` to compute its assigned chunk. So this is not a genuine domain decomposition

## Build for CPU (Kokkos::OpenMP) on hpcai

```shell
# if using OpenMP as default backend
module load kokkos/4.1.00-openmp-gnu-9.4.0-RelWithDebInfo
mkdir -p _build/openmp
cd _build/openmp
cmake ../..
make
```

## Build for GPU (Kokkos::Cuda) on hpcai

```shell
# if using Cuda as default backend
module load kokkos/4.1.00-cuda-11.8-gnu-9.4.0-RelWithDebInfo
mkdir -p _build/cuda
cd _build/cuda
cmake ../..
make
```

# Possible activities

1. implement 2D paralelization (along X and Y axis)
2. change both memory layout and Kokkos::MDRangePolicy template `iterate` parameter for e.g. the OpenMP backend. See how it affects performance.
   more precisely, for Kokkos::OpenMP, default memory layout is RightLayout, and default Kokkos::Iterate is Kokkos::Iterate::Right
