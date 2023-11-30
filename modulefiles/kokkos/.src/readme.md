# Kokkos sources

**WARNING:**

These are the instructions to build kokkos on the kraken supercomputer; they should be slightly adapted when used on a different machine. Espacially the architecture flags can/should be tuned for the actual CPU/GPU available architecture.

Retrieve the latest available release of kokkos : 4.1.00

```shell
git clone git@github.com:kokkos/kokkos.git
cd kokkos
git checkout 4.1.00
```

# Build Kokkos for OpenMP with gnu tool toolchain

```shell
cd kokkos

export GNU_VERSION=11.2.0
export KOKKOS_VERSION=4.1.00

CMAKE_BUILD_TYPE=RelWithDebInfo

module load compiler/gcc/$GNU_VERSION
module load lib/hwloc/2.1.0

BUILD_DIR=_build/$KOKKOS_VERSION/openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/kokkos-$KOKKOS_VERSION-openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE

mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake -DKokkos_ENABLE_OPENMP=ON -DKokkos_ENABLE_HWLOC=ON -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE ../../..
make -j 8
make install
```

# Build Kokkos for OpenMP and CUDA with gnu tool toolchain

```shell
cd kokkos

export GNU_VERSION=11.2.0
export KOKKOS_VERSION=4.1.00
export CUDA_VERSION=12.0

CMAKE_BUILD_TYPE=RelWithDebInfo

module load compiler/gcc/$GNU_VERSION
module load nvidia/cuda/$CUDA_VERSION

BUILD_DIR=_build/$KOKKOS_VERSION/cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/kokkos-$KOKKOS_VERSION-cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# note: on kraken, when building on login node, kokkos can't auto detect GPU architecture, so we provide it
cmake -DKokkos_ENABLE_OPENMP=ON -DKokkos_ENABLE_HWLOC=ON -DCMAKE_CXX_STANDARD=17 -DKokkos_ENABLE_CUDA=ON -DKokkos_ARCH_AMPERE80=ON -DKokkos_ENABLE_CUDA_LAMBDA=ON -DKokkos_ENABLE_CUDA_CONSTEXPR=ON -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE ../../..
make -j 8
make install
```

# Build Kokkos for OpenMP with Intel/LLVM toolchain

The benefit of this build is that will have vectorization flag support.

Kraken CPU is an Intel Xeon processor (we know it by running `lscpu`), so we can instruct kokkos build to use compilation flag specific to that architecture.

**Note**

Be carefull, the following instructions are specific to Kraken (espacially regarding the compiler modulefile). We still need to load the gnu compiler, because intel compiler relies on GNU header for standard library.

**Build instructions**

```shell
cd kokkos

export GNU_VERSION=11.2.0
export INTEL_LLVM_VERSION=23.2.1
export KOKKOS_VERSION=4.1.00

CMAKE_BUILD_TYPE=RelWithDebInfo

module load compiler/gcc/$GNU_VERSION
unset CXX CC F77 F90 F95 FC

module load compiler/intel/$INTEL_LLVM_VERSION
export CXX=icpx

module load lib/hwloc/2.1.0

BUILD_DIR=_build/$KOKKOS_VERSION/openmp-intel-llvm-$INTEL_LLVM_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/kokkos-$KOKKOS_VERSION-openmp-intel-llvm-$INTEL_LLVM_VERSION-$CMAKE_BUILD_TYPE
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake -DKokkos_ENABLE_OPENMP=ON -DKokkos_ENABLE_HWLOC=ON -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE ../../..
make -j 8
make install
```
