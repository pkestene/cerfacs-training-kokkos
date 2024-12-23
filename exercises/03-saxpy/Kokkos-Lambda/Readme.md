# Build with Makefile

## Kokkos::OpenMP backend

```shell
make -j 8 KOKKOS_PATH=$HOME/install/kokkos KOKKOS_ARCH=ARMv9-Grace
```

## Kokkos::Cuda backend

```shell
module load nvidia/cuda/12.4
make -j 8 KOKKOS_PATH=$HOME/install/kokkos KOKKOS_ARCH=ARMv9-Grace,Hopper90 KOKKOS_DEVICES=OpenMP,Cuda
```

# Build with cmake and modulefile

## Kokkos::OpenMP backend

```shell
module use $HOME/cerfacs-training-kokkos/modulefiles
module load kokkos/4.4.01-openmp-gnu-12.3.0-RelWithDebInfo
mkdir -p _build/openmp
cd _build/openmp

cmake ../..
make -j 8
```

## Kokkos::Cuda backend

```shell
module use $HOME/cerfacs-training-kokkos/modulefiles
module load kokkos/4.4.01-cuda-12.4-gnu-12.3.0-RelWithDebInfo
mkdir -p _build/cuda
cd _build/cuda

cmake ../..
make -j 8
```

# Build with cmake both kokkos and application

## Kokkos::OpenMP backend

```shell
mkdir -p _build/openmp2
cd _build/openmp2

cmake -DSAXPY_KOKKOS_BUILD=ON -DSAXPY_KOKKOS_BACKEND=OpenMP ../..
make -j 8
```

## Kokkos::Cuda backend

```shell
mkdir -p _build/cuda2
cd _build/cuda2

module load nvidia/cuda/12.4
cmake -DSAXPY_KOKKOS_BUILD=ON -DSAXPY_KOKKOS_BACKEND=Cuda ../..
make -j 8
```

### Additionnal activity:

With this version, cross-check that you can change the default execution space. Eventhough, Cuda is activated (so it should be the default execution space), you can still, if you like, enforce OpenMP as the default execution space. Do the modification below, and check that OpenMP is the default execution space.

```cmake
target_compile_definitions(saxpy PRIVATE KOKKOS_ENABLE_DEFAULT_DEVICE_TYPE_OPENMP)
```
