Here we build kokkos using the cmake build system. We will explore two options:
- build and install kokkos so that it can be re-used later when building an application or a large project
- show how to integrate kokkos build into your application build system (need a fairly good understanding of cmake)

We are loosely following [Kokkos lecture 1](https://github.com/kokkos/kokkos-tutorials/blob/main/LectureSeries/KokkosTutorial_01_Introduction.pdf), page 54 and after.

# Minimal environment for calypso

```shell
# allocate a interactive session on a Grace computing node
salloc --partition=grace -N 1 --time=1:0:0 --gres=gpu:1 -n 24

# put this at the end of your .bashrc on calypso login node
# each time you log onto a grace computing node, the following environment will be loaded:
HOST=`hostname`
case "$HOST" in
    # calypso grace partition node
    calypso-grace*) module load gcc/12.3.0_arm tools/cmake/3.29.3_arm nvidia/cuda/12.4 ;;
esac
```

# Download Kokkos sources

Here are basic steps to download kokkos sources.

```shell
mkdir $HOME/install
cd $HOME/install
git clone git@github.com:kokkos/kokkos.git
cd kokkos
# we will use the latest kokkos release to date (mid November 2024)
git checkout 4.4.01
```

# Exploring Kokkos cmake options

```shell
cd kokkos
export KOKKOS_VERSION=4.4.01
BUILD_DIR=_build/$KOKKOS_VERSION/tmp
mkdir -p $BUILD_DIR
cd $BUILD_DIR
ccmake ../../..
# we will discuss all the options during hands-on
```

It is good practice to have a look at file `KokkosCore_config.h` generated at top-level of the build folder. It contains a list of all the features enabled/disabled; it good to cross-check that the features you wanted at effectively enabled.

# Build Kokkos for OpenMP backend with the gnu tool toolchain

The following commands can be run
- either on a Grace compute node of calypso (Grace is a ARM CPU).
- either on a calypso login node (x86-64 Intel architecture)

```shell
cd kokkos

export GNU_VERSION=12.3.0
export KOKKOS_VERSION=4.4.01

CMAKE_BUILD_TYPE=RelWithDebInfo

CPU_ARCH=`uname -p`

if [ "$CPU_ARCH" = "aarch64" ]; then
    module load gcc/${GNU_VERSION}_arm
else
    # x64-64 build
    module load gcc/${GNU_VERSION}
fi

BUILD_DIR=_build/$CPU_ARCH/$KOKKOS_VERSION/openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/$CPU_ARCH/kokkos-$KOKKOS_VERSION-openmp-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake -DKokkos_ENABLE_OPENMP=ON -DKokkos_ENABLE_HWLOC=ON -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE ../../../..
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
hwloc( NUMA[2] x CORE[72] x HT[1] )
  Kokkos Version: 4.4.1
Compiler:
  KOKKOS_COMPILER_GNU: 1230
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
Options:
  KOKKOS_ENABLE_ASM: no
  KOKKOS_ENABLE_CXX17: yes
  KOKKOS_ENABLE_CXX20: no
  KOKKOS_ENABLE_CXX23: no
  KOKKOS_ENABLE_CXX26: no
  KOKKOS_ENABLE_DEBUG_BOUNDS_CHECK: no
  KOKKOS_ENABLE_HWLOC: yes
  KOKKOS_ENABLE_LIBDL: yes
Host Parallel Execution Space:
  KOKKOS_ENABLE_OPENMP: yes

OpenMP Runtime Configuration:
Kokkos::OpenMP thread_pool_topology[ 1 x 72 x 1 ]
}
```


# Build Kokkos for both OpenMP and CUDA backends with gnu tool toolchain and nvcc toolchain

The following commands must be run on a Grace compute node of calypso.

```shell
cd kokkos

#unset CXX
export GNU_VERSION=12.3.0
export KOKKOS_VERSION=4.4.01
export CUDA_VERSION=12.4

CMAKE_BUILD_TYPE=RelWithDebInfo

CPU_ARCH=`uname -p`

if [ "$CPU_ARCH" = "aarch64" ]; then
    module load gcc/${GNU_VERSION}_arm
    module load tools/cmake/3.29.3_arm
    module load nvidia/cuda/$CUDA_VERSION
    module load lib/hwloc/2.11.2
else
    echo "ERROR: this must be done on a Grace computing node"
fi

BUILD_DIR=_build/$CPU_ARCH/$KOKKOS_VERSION/cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
INSTALL_DIR=$HOME/local/$CPU_ARCH/kokkos-$KOKKOS_VERSION-cuda-$CUDA_VERSION-gnu-$GNU_VERSION-$CMAKE_BUILD_TYPE
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -DKokkos_ENABLE_OPENMP=ON -DKokkos_ENABLE_HWLOC=ON -DCMAKE_CXX_STANDARD=17 -DKokkos_ENABLE_CUDA=ON -DKokkos_ENABLE_CUDA_CONSTEXPR=ON -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE ../../../..
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
# run this example directly on the grace node
```

What differences do you see in the output of the job ?

**Additionnal notes**
- please note that, by enabling the CUDA backend, kokkos build system automatically switch the compiler to `nvcc_wrapper` which itself is a shell script wrapping the use of Nvidia compiler `nvcc`
- Kokkos CUDA backend can also be compiled using another toolchain: `nvhpc` or `clang`
