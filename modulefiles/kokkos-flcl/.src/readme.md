# Get [Kokkos-FLCL](https://github.com/kokkos/kokkos-fortran-interop) sources

[Kokkos FLCL (Fortran Language Compatibility Layer)](https://github.com/kokkos/kokkos-fortran-interop)

```shell
git clone git@github.com:kokkos/kokkos-fortran-interop.git
```

# Build for openmp

```shell
cd kokkos-fortran-interop

GNU_VERSION=11.2.0
KOKKOS_VERSION=4.1.00

CMAKE_BUILD_TYPE=RelWithDebInfo

module load compiler/gcc/$GNU_VERSION
module load lib/hwloc/2.1.0
module load kokkos/$KOKKOS_VERSION-openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE

BUILD_DIR=_build/$KOKKOS_VERSION-openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/kokkos-flcl-$KOKKOS_VERSION-openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -DCMAKE_BUILD_TYPE=Release -DFLCL_BUILD_EXAMPLES=ON -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE ../..
make -j 8
make install
```

# Build for cuda

```shell
cd kokkos-fortran-interop

GNU_VERSION=11.2.0
CUDA_VERSION=12.0
KOKKOS_VERSION=4.1.00

CMAKE_BUILD_TYPE=RelWithDebInfo

module load compiler/gcc/$GNU_VERSION
module load lib/hwloc/2.1.0
module load nvidia/cuda/$CUDA_VERSION
module load kokkos/$KOKKOS_VERSION-cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE

BUILD_DIR=_build/$KOKKOS_VERSION-cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/kokkos-flcl-$KOKKOS_VERSION-cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -DCMAKE_BUILD_TYPE=Release -DFLCL_BUILD_EXAMPLES=ON -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE ../..
make -j 8
make install
```
