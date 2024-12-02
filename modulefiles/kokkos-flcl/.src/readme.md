# Get [Kokkos-FLCL](https://github.com/kokkos/kokkos-fortran-interop) sources

[Kokkos FLCL (Fortran Language Compatibility Layer)](https://github.com/kokkos/kokkos-fortran-interop)

```shell
git clone git@github.com:kokkos/kokkos-fortran-interop.git
```

# Build for openmp

```shell
cd kokkos-fortran-interop

GNU_VERSION=12.3.0
KOKKOS_VERSION=4.4.01

CMAKE_BUILD_TYPE=RelWithDebInfo

ARCH_CPU=`uname -p`
case "$ARCH_CPU" in
    aarch64*) module load gcc/${GNU_VERSION}_arm tools/cmake/3.29.3_arm lib/hwloc/2.11.2 kokkos/$KOKKOS_VERSION-openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE ;;
esac

BUILD_DIR=_build/${ARCH_CPU}/$KOKKOS_VERSION-openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/${ARCH_CPU}/kokkos-flcl-$KOKKOS_VERSION-openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DFLCL_BUILD_EXAMPLES=ON \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
    -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
    ../../..
make -j 8
make install
```

# Build for cuda

```shell
cd kokkos-fortran-interop

GNU_VERSION=12.3.0
CUDA_VERSION=12.4
KOKKOS_VERSION=4.4.01

CMAKE_BUILD_TYPE=RelWithDebInfo

ARCH_CPU=`uname -p`
case "$ARCH_CPU" in
    aarch64*) module load gcc/${GNU_VERSION}_arm tools/cmake/3.29.3_arm lib/hwloc/2.11.2 nvidia/cuda/$CUDA_VERSION kokkos/$KOKKOS_VERSION-cuda-${CUDA_VERSION}-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE ;;
esac

BUILD_DIR=_build/${ARCH_CPU}/$KOKKOS_VERSION-cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/${ARCH_CPU}/kokkos-flcl-$KOKKOS_VERSION-cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DFLCL_BUILD_EXAMPLES=ON \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
    -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
    ../../..
make -j 8
make install
```
