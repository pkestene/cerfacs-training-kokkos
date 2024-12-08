# Kokkos sources

**WARNING:**

These are the instructions to build kokkos on the kraken supercomputer; they should be slightly adapted when used on a different machine. Especially the architecture flags can/should be tuned for the actual CPU/GPU available architecture.

Retrieve the latest available release of kokkos : 4.4.01

```shell
git clone git@github.com:kokkos/kokkos.git
cd kokkos
git checkout 4.4.01
```

# Build Kokkos for OpenMP with gnu tool toolchain

**note**
When using cmake flag `-DKokkos_ARCH_ARMV9_GRACE=ON`, the compiler flags will be enriched with `-mcpu=neoverse-n2 -msve-vector-bits=128`

```shell
cd kokkos

export GNU_VERSION=12.3.0
export KOKKOS_VERSION=4.4.01

CMAKE_BUILD_TYPE=RelWithDebInfo

ARCH_CPU=`uname -p`
case "$ARCH_CPU" in
    aarch64*) module load gcc/${GNU_VERSION}_arm tools/cmake/3.29.3_arm lib/hwloc/2.11.2 ;;
    x86-64*) module load gcc/${GNU_VERSION} tools/cmake/3.29.3 lib/hwloc/2.11.2 ;;
esac

BUILD_DIR=_build/${ARCH_CPU}/$KOKKOS_VERSION/openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/${ARCH_CPU}/kokkos-$KOKKOS_VERSION-openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE

mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake \
    -DKokkos_ENABLE_OPENMP=ON \
    -DKokkos_ENABLE_HWLOC=ON \
    -DKokkos_ARCH_ARMV9_GRACE=ON \
    -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
    -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
    ../../../..
make -j 8
make install
```

# Build Kokkos for OpenMP and CUDA with gnu tool toolchain

```shell
cd kokkos

export GNU_VERSION=12.3.0
export KOKKOS_VERSION=4.4.01
export CUDA_VERSION=12.4

CMAKE_BUILD_TYPE=RelWithDebInfo

ARCH_CPU=`uname -p`
case "$ARCH_CPU" in
    aarch64*) module load gcc/${GNU_VERSION}_arm tools/cmake/3.29.3_arm lib/hwloc/2.11.2 nvidia/cuda/$CUDA_VERSION ;;
    x86-64*) module load gcc/${GNU_VERSION} tools/cmake/3.29.3 lib/hwloc/2.11.2 ;;
esac

BUILD_DIR=_build/${ARCH_CPU}/$KOKKOS_VERSION/cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/${ARCH_CPU}/kokkos-$KOKKOS_VERSION-cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# note: on calypso, when building on grace compute node, GPU architecture is auto-detected
# (when auto detection is not available, one may need to specify it
# e.g. -DKokkos_ARCH_HOPPER90=ON)
cmake \
    -DKokkos_ENABLE_OPENMP=ON \
    -DKokkos_ENABLE_HWLOC=ON \
    -DKokkos_ARCH_ARMV9_GRACE=ON \
    -DCMAKE_CXX_STANDARD=17 \
    -DKokkos_ENABLE_CUDA=ON  \
    -DKokkos_ENABLE_CUDA_LAMBDA=ON \
    -DKokkos_ENABLE_CUDA_CONSTEXPR=ON \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
    -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
    ../../../..
make -j 8
make install
```

# Build Kokkos for OpenMP with NVHPC tool toolchain

```shell
cd kokkos

export NVHPC_VERSION=24.7
export KOKKOS_VERSION=4.4.01

CMAKE_BUILD_TYPE=RelWithDebInfo

ARCH_CPU=`uname -p`
case "$ARCH_CPU" in
    aarch64*) module load nvhpc/${NVHPC_VERSION} tools/cmake/3.29.3_arm lib/hwloc/2.11.2 ;;
esac

BUILD_DIR=_build/${ARCH_CPU}/$KOKKOS_VERSION/openmp-nvhpc-$NVHPC_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/${ARCH_CPU}/kokkos-$KOKKOS_VERSION-openmp-nvhpc-$NVHPC_VERSION-$CMAKE_BUILD_TYPE

mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake \
    -DKokkos_ENABLE_OPENMP=ON \
    -DKokkos_ENABLE_HWLOC=ON \
    -DKokkos_ARCH_ARMV9_GRACE=ON \
    -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
    -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
    ../../../..
make -j 8
make install
```

# Build Kokkos for Cuda with NVHPC tool toolchain

```shell
cd kokkos

export NVHPC_VERSION=24.7
export KOKKOS_VERSION=4.4.01

CMAKE_BUILD_TYPE=RelWithDebInfo

ARCH_CPU=`uname -p`
case "$ARCH_CPU" in
    aarch64*) module load nvhpc/${NVHPC_VERSION} tools/cmake/3.29.3_arm lib/hwloc/2.11.2 ;;
esac

BUILD_DIR=_build/${ARCH_CPU}/$KOKKOS_VERSION/cuda-nvhpc-$NVHPC_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/${ARCH_CPU}/kokkos-$KOKKOS_VERSION-cuda-nvhpc-$NVHPC_VERSION-$CMAKE_BUILD_TYPE

mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake \
    -DKokkos_ENABLE_OPENMP=ON \
    -DKokkos_ENABLE_CUDA=ON \
    -DKokkos_ENABLE_HWLOC=ON \
    -DKokkos_ARCH_ARMV9_GRACE=ON \
    -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
    -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
    ../../../..
make -j 8
make install
```
