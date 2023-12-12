Here we build kokkos using the cmake build system. We will explore two options:
- build and install kokkos so that it can be re-used later when building an application or a large project
- show how to integrate kokkos build into your application build system (need a fairly good understanding of cmake)

We are loosely following [Kokkos lecture 1](https://github.com/kokkos/kokkos-tutorials/blob/main/LectureSeries/KokkosTutorial_01_Introduction.pdf), page 54 and after.

# Minimal environment for Kraken

```shell
# put this at the end of your .bash_profile on kraken
# we want to use a fairly recent version of the GNU compiler
module unload compiler
module load compiler/gcc/11.2.0

module unload mpi

module unload tools/cmake
module load tools/cmake/3.23.0

module unload tools/git
module load tools/git/2.37.1
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
export GNU_VERSION=11.2.0
export KOKKOS_VERSION=4.1.00

CMAKE_BUILD_TYPE=RelWithDebInfo

module load compiler/gcc/$GNU_VERSION
module load lib/hwloc/2.1.0

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
# copy the template job.sh script (from top level exercise folder) and run it like this
sbatch job.sh
```

You should have an output similar to this (that we will comment during hands-on session)

```text
Kokkos::OpenMP::initialize WARNING: OMP_PROC_BIND environment variable not set
  In general, for best performance with OpenMP 4.0 or better set OMP_PROC_BIND=spread and OMP_PLACES=threads
  For best performance with OpenMP 3.1 set OMP_PROC_BIND=true
  For unit testing set OMP_PROC_BIND=false

MPI detected: For OpenMP binding to work as intended, MPI ranks must be bound to exclusive CPU sets.

{
hwloc( NUMA[2] x CORE[16] x HT[1] )
  Kokkos Version: 4.1.0
Compiler:
  KOKKOS_COMPILER_GNU: 1120
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
Kokkos::OpenMP thread_pool_topology[ 1 x 32 x 1 ]
}
```

# Build Kokkos for OpenMP backend with Intel/LLVM toolchain

The benefit of this build is that it will bring vectorization flag for the CPU execution on Intel Xeon processor. For this to work, we need to use Intel/LLVM compiler version >= 2023.1.0

Kraken CPU is an Intel Xeon processor (we know it by running `lscpu`), so we can instruct kokkos build to use compilation flag specific to that architecture.

**Note**

Be carefull, the following instructions are specific to Kraken (espacially regarding the compiler modulefile). We still need to load the gnu compiler, because intel compiler relies on GNU header for standard library.

**Build instructions**

```shell
export GNU_VERSION=11.2.0
export INTEL_LLVM_VERSION=23.2.1
export KOKKOS_VERSION=4.1.00

CMAKE_BUILD_TYPE=RelWithDebInfo

module load compiler/gcc/$GNU_VERSION
unset CXX CC F77 F90 F95 FC

module load compiler/intel/$INTEL_LLVM_VERSION
export CXX=icpx

module load lib/hwloc/2.1.0

cd kokkos
BUILD_DIR=_build/$KOKKOS_VERSION/openmp-intel-llvm-$INTEL_LLVM_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/kokkos-$KOKKOS_VERSION-openmp-intel-llvm-$INTEL_LLVM_VERSION-$CMAKE_BUILD_TYPE
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake -DKokkos_ENABLE_OPENMP=ON -DKokkos_ENABLE_HWLOC=ON -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE ../../..
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
# copy the template job.sh script (from top level exercise folder) and run it like this
sbatch job.sh
```
What differences do you see in the output of the job ?

# Build Kokkos for both OpenMP and CUDA backends with gnu tool toolchain and nvcc toolchain

```shell
unset CXX
export GNU_VERSION=11.2.0
export KOKKOS_VERSION=4.1.00
export CUDA_VERSION=12.0

CMAKE_BUILD_TYPE=RelWithDebInfo

module load compiler/gcc/$GNU_VERSION
module load nvidia/cuda/$CUDA_VERSION

cd kokkos
BUILD_DIR=_build/$KOKKOS_VERSION/cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/kokkos-$KOKKOS_VERSION-cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# note: on kraken, when building on login node, kokkos can't auto detect GPU architecture, so we provide it
cmake -DKokkos_ENABLE_OPENMP=ON -DKokkos_ENABLE_HWLOC=ON -DCMAKE_CXX_STANDARD=17 -DKokkos_ENABLE_CUDA=ON -DKokkos_ARCH_AMPERE80=ON -DKokkos_ENABLE_CUDA_CONSTEXPR=ON -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE ../../..
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
# copy the template job.sh script (from top level exercise folder) and run it like this
sbatch job.sh
```

What differences do you see in the output of the job ?

**Additionnal notes**
- please note that, by enabling the CUDA backend, kokkos build system automatically switch the compiler to `nvcc_wrapper` which itself is a shell script wrapping the use of Nvidia compiler `nvcc`
- Kokkos CUDA backend can also be compiled using another toolchain: nvhpc or clang
