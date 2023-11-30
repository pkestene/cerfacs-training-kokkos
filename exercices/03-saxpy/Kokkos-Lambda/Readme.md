# Build with Makefile

## Kokkos::OpenMP backend

```shell
make -j 8 KOKKOS_PATH=$HOME/install/kokkos KOKKOS_ARCH=SKX
```

## Kokkos::Cuda backend

```shell
module load nvidia/cuda/12.0
make -j 8 KOKKOS_PATH=$HOME/install/kokkos KOKKOS_ARCH=SKX,Ampere80 KOKKOS_DEVICES=OpenMP,Cuda
```

# Build with cmake and modulefile

## Kokkos::OpenMP backend

```shell
module $HOME/cerfacs-training-kokkos/modulefiles
module load kokkos/4.1.00-openmp-gnu-11.2.00-RelWithDebInfo
mkdir -p _build/openmp
cd _build/openmp

cmake ../..
make -j 8
```

## Kokkos::Cuda backend

```shell
module $HOME/cerfacs-training-kokkos/modulefiles
module load kokkos/4.1.00-cuda-12.0-gnu-11.2.00-RelWithDebInfo
mkdir -p _build/cuda
cd _build/cuda

cmake ../..
make -j 8
```

# Build with cmake both kokkos and application

## Kokkos::OpenMP backend

```shell
mkdir -p _build/openmp
cd _build/openmp2

cmake -DSAXPY_KOKKOS_BUILD=ON -DSAXPY_KOKKOS_BACKEND=OpenMP ../..
make -j 8
```

## Kokkos::Cuda backend

```shell
mkdir -p _build/cuda
cd _build/cuda2

module load nvidia/cuda/12.0
cmake -DSAXPY_KOKKOS_BUILD=ON -DSAXPY_KOKKOS_BACKEND=Cuda ../..
make -j 8
```

### Additionnal activity:

With this version, cross-check that you can change the default execution space. Eventhough, Cuda is activated (so it should be the default execution space), you can still, if you like, enforce OpenMP as the default execution space. Do the modification below, and check that OpenMP is the default execution space.

```cmake
target_compile_definitions(saxpy PRIVATE KOKKOS_ENABLE_DEFAULT_DEVICE_TYPE_OPENMP)
```
