# Kokkos-Kernels sources

**WARNING:**

These are the instructions to build kokkos on the kraken supercomputer; they should be slightly adapted when used on a different machine. Espacially the architecture flags can/should be tuned for the actual CPU/GPU available architecture.

Retrieve the latest available release of kokkos-kernels : 4.1.00

```shell
git clone git@github.com:kokkos/kokkos-kernels.git
cd kokkos-kernels
git checkout 4.1.00
```

# Build Kokkos-Kernels for OpenMP with gnu tool toolchain

```shell
cd kokkos-kernels

export GNU_VERSION=11.2.0
export KOKKOS_VERSION=4.1.00
export KOKKOS_KERNELS_VERSION=4.1.00

CMAKE_BUILD_TYPE=RelWithDebInfo

module load compiler/gcc/$GNU_VERSION
module load lib/hwloc/2.1.0
module load kokkos/$KOKKOS_VERSION-openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE

BUILD_DIR=_build/$KOKKOS_KERNELS_VERSION/openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/kokkos-kernels-$KOKKOS_KERNELS_VERSION-openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE

mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake -DKokkosKernels_ENABLE_EXAMPLES=ON -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE ../../..
make -j 8
make install
```

# Build Kokkos-Kernels for OpenMP and CUDA with gnu tool toolchain

```shell
cd kokkos-kernels

export GNU_VERSION=11.2.0
export KOKKOS_VERSION=4.1.00
export KOKKOS_KERNELS_VERSION=4.1.00
export CUDA_VERSION=12.0

CMAKE_BUILD_TYPE=RelWithDebInfo

module load compiler/gcc/$GNU_VERSION
module load kokkos/$KOKKOS_VERSION-cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
module load nvidia/cuda/$CUDA_VERSION

BUILD_DIR=_build/$KOKKOS_KERNELS_VERSION/cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/kokkos-kernels-$KOKKOS_KERNELS_VERSION-cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE

mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake -DKokkosKernels_ENABLE_EXAMPLES=ON -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE ../../..
make -j 6
make install
```
