Here we build kokkos using the cmake build system. We will explore two options:
- build and install kokkos so that it can be re-used later when building an application or a large project
- show how to integrate kokkos build into your application build system (need a fairly good understanding of cmake)

We are loosely following [Kokkos lecture 1](https://github.com/kokkos/kokkos-tutorials/blob/main/LectureSeries/KokkosTutorial_01_Introduction.pdf), page 54 and after.

# Minimal environment for hpcai

```shell
# put this at the end of your .bash_profile on hpcai
# we want to use a fairly recent version of the GNU compiler
module unload tools/cmake
module load tools/cmake/3.25.0
```

# Download Kokkos sources

Here are basic steps to download kokkos sources.

```shell
mkdir $HOME/install
cd $HOME/install
git clone git@github.com:kokkos/kokkos.git
cd kokkos
# we will use the latest kokkos release to date (mid November 2023)
git checkout 4.1.00
```

# Exploring Kokkos cmake options

```shell
cd kokkos
export KOKKOS_VERSION=4.1.00
BUILD_DIR=_build/$KOKKOS_VERSION/tmp
mkdir -p $BUILD_DIR
cd $BUILD_DIR
ccmake ../../..
# we will discuss all the options during hands-on
```

It is good practice to have a look at file `KokkosCore_config.h` generated at top-level of the build folder. It contains a list of all the features enabled/disabled; it good to cross-check that the features you wanted at effectively enabled.

# Build Kokkos for OpenMP backend with the gnu tool toolchain

```shell
export GNU_VERSION=9.4.0
export KOKKOS_VERSION=4.1.00

CMAKE_BUILD_TYPE=RelWithDebInfo

cd kokkos
BUILD_DIR=_build/$KOKKOS_VERSION/openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/kokkos-$KOKKOS_VERSION-openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake -DKokkos_ENABLE_OPENMP=ON -DKokkos_ENABLE_HWLOC=ON -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE ../../..
make -j 8
make install
```

As a simple test, lets rebuild kokkos and activate examples:

```shell
ccmake ../../..
# turn Kokkos_ENABLE_EXAMPLES=ON
# press "c" to configure
# press "g" to generate Makefile
make -j 8
cd example/query_device
./Kokkos_query_device
```

You should have an output similar to this (that we will comment during hands-on session)

```text
Kokkos::OpenMP::initialize WARNING: OMP_PROC_BIND environment variable not set
  In general, for best performance with OpenMP 4.0 or better set OMP_PROC_BIND=spread and OMP_PLACES=threads
  For best performance with OpenMP 3.1 set OMP_PROC_BIND=true
  For unit testing set OMP_PROC_BIND=false

{
hwloc( NUMA[1] x CORE[32] x HT[2] )
  Kokkos Version: 4.1.0
Compiler:
  KOKKOS_COMPILER_GNU: 940
Architecture:
  CPU architecture: none
  Default Device: N6Kokkos6OpenMPE
  GPU architecture: none
  platform: 64bit
Atomics:
Vectorization:
  KOKKOS_ENABLE_PRAGMA_IVDEP: no
  KOKKOS_ENABLE_PRAGMA_LOOPCOUNT: no
  KOKKOS_ENABLE_PRAGMA_UNROLL: no
  KOKKOS_ENABLE_PRAGMA_VECTOR: no
Memory:
  KOKKOS_ENABLE_HBWSPACE: no
  KOKKOS_ENABLE_INTEL_MM_ALLOC: no
Options:
  KOKKOS_ENABLE_ASM: yes
  KOKKOS_ENABLE_CXX17: yes
  KOKKOS_ENABLE_CXX20: no
  KOKKOS_ENABLE_CXX23: no
  KOKKOS_ENABLE_DEBUG_BOUNDS_CHECK: no
  KOKKOS_ENABLE_HWLOC: yes
  KOKKOS_ENABLE_LIBDL: yes
  KOKKOS_ENABLE_LIBRT: no
Host Parallel Execution Space:
  KOKKOS_ENABLE_OPENMP: yes

OpenMP Runtime Configuration:
Kokkos::OpenMP thread_pool_topology[ 1 x 64 x 1 ]
}
```

# Build Kokkos for both OpenMP and CUDA backends with gnu tool toolchain and nvcc toolchain

```shell
export GNU_VERSION=9.4.0
export KOKKOS_VERSION=4.1.00
export CUDA_VERSION=11.8

CMAKE_BUILD_TYPE=RelWithDebInfo

cd kokkos
BUILD_DIR=_build/$KOKKOS_VERSION/cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/kokkos-$KOKKOS_VERSION-cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# note: on hpcai, kokkos is able to auto-detect GPU hardware architecture (i.e. AMpere80 here).
# when auto-detection is not possible, you to explicitely pass the hardware option to cmake, e.g. -DKokkos_ARCH_AMPERE80=ON
cmake -DKokkos_ENABLE_OPENMP=ON -DKokkos_ENABLE_HWLOC=ON -DCMAKE_CXX_STANDARD=17 -DKokkos_ENABLE_CUDA=ON -DKokkos_ENABLE_CUDA_CONSTEXPR=ON -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE ../../..
make -j 8
make install
```

Again, rebuild kokkos to enable examples:
```shell
ccmake ../../..
# turn Kokkos_ENABLE_EXAMPLES=ON
# press "c" to configure
# press "g" to generate Makefile
make -j 8
cd example/query_device
./Kokkos_query_device
```

What differences do you see in the output of the job ?

**Additionnal notes**
- please note that, by enabling the CUDA backend, kokkos build system automatically switch the compiler to `nvcc_wrapper` which itself is a shell script wrapping the use of Nvidia compiler `nvcc`
- Kokkos CUDA backend can also be compiled using another toolchain: nvhpc or clang
