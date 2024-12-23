# Compute pi with monte carlo method.

- subdir src contains the code to use for the exercise (fill the holes).
- subdir solution is complete.

If you just want to build solution, just add "-DBUILD_SOLUTION=ON" on the cmake command line

# Activity

- open file `src/compute_pi.cpp` and try to fill the holes.

# How to build ?

## Using kokkos modulefile

### Kokkos::OpenMP backend

```shell
module load kokkos/4.4.01-openmp-gnu-12.3.0-RelWithDebInfo
mkdir -p _build/openmp; cd _build/openmp
cmake ../..
make
```

### Kokkos::Cuda backend

```shell
module load kokkos/4.4.01-cuda-12.4-gnu-12.3.0-RelWithDebInfo
mkdir -p _build/cuda; cd _build/cuda
cmake ../..
make
```

## Download Kokkos soruces as part of the build

### Kokkos::OpenMP backend

```shell
mkdir -p _build/openmp; cd _build/openmp
cmake -DCOMPUTEPI_KOKKOS_BUILD=ON -DCOMPUTEPI_KOKKOS_BACKEND=OpenMP ../..
make
```

### Kokkos::Cuda backend

```shell
mkdir -p _build/cuda; cd _build/cuda
cmake -DCOMPUTEPI_KOKKOS_BUILD=ON -DCOMPUTEPI_KOKKOS_BACKEND=Cuda ../..
make
```
