# Kokkos-Kernels sources

**WARNING:**

These are the instructions to build kokkos on the kraken supercomputer; they should be slightly adapted when used on a different machine. Espacially the architecture flags can/should be tuned for the actual CPU/GPU available architecture.

Retrieve the latest available release of kokkos-kernels : 4.1.00

```shell
git clone git@github.com:kokkos/kokkos-kernels.git
cd kokkos-kernels
git checkout 4.4.01
```

# Build Kokkos-Kernels for OpenMP with gnu tool toolchain

```shell
cd kokkos-kernels

export GNU_VERSION=12.3.0
export KOKKOS_VERSION=4.4.01
export KOKKOS_KERNELS_VERSION=4.4.01

CMAKE_BUILD_TYPE=RelWithDebInfo

ARCH_CPU=`uname -p`
case "$ARCH_CPU" in
    aarch64*) module load gcc/${GNU_VERSION}_arm tools/cmake/3.29.3_arm lib/hwloc/2.11.2 kokkos/$KOKKOS_VERSION-openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE ;;
esac

BUILD_DIR=_build/${ARCH_CPU}/$KOKKOS_KERNELS_VERSION/openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/${ARCH_CPU}/kokkos-kernels-$KOKKOS_KERNELS_VERSION-openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE

mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake \
    -DKokkosKernels_ENABLE_EXAMPLES=ON \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
    -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
    ../../../..
make -j 16
make install
```

# Build Kokkos-Kernels for OpenMP and CUDA with gnu tool toolchain

```shell
cd kokkos-kernels

export GNU_VERSION=12.3.0
export KOKKOS_VERSION=4.4.01
export KOKKOS_KERNELS_VERSION=4.4.01
export CUDA_VERSION=12.4

CMAKE_BUILD_TYPE=RelWithDebInfo

ARCH_CPU=`uname -p`
case "$ARCH_CPU" in
    aarch64*) module load gcc/${GNU_VERSION}_arm tools/cmake/3.29.3_arm lib/hwloc/2.11.2 nvidia/cuda/$CUDA_VERSION kokkos/$KOKKOS_VERSION-cuda-${CUDA_VERSION}-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE ;;
esac

BUILD_DIR=_build/${ARCH_CPU}/$KOKKOS_KERNELS_VERSION/cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/${ARCH_CPU}/kokkos-kernels-$KOKKOS_KERNELS_VERSION-cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE

mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake \
    -DKokkosKernels_ENABLE_EXAMPLES=ON \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
    -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
    ../../../..
make -j 16
make install
```
