# Get [YAKL](https://github.com/mrnorman/YAKL) sources

```shell
git clone git@github.com:mrnorman/YAKL.git
```

# Build for OpenMP

Example of yakl build recipe using e.g. OPENMP as a target architecture (YAKL_ARCH).

```shell
export GNU_VERSION=11.2.0

module load compiler/gcc/$GNU_VERSION
module load lib/hwloc/2.1.0
module load openmpi/4.1.6-gnu-11.2.0-cuda-12.0

BUILD_DIR=_build/openmp-gnu-$GNU_VERSION
INSTALL_DIR=$HOME/local/yakl-openmp-gnu-$GNU_VERSION

mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -DYAKL_ARCH=OPENMP -DYAKL_HAVE_MPI=ON -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR ../..

make
make install
```

# Build for Cuda

Example of yakl build recipe using e.g. CUDA as a target architecture (YAKL_ARCH).

```shell
export GNU_VERSION=11.2.0
export CUDA_VERSION=12.0
export CUDA_ARCH=80

module load compiler/gcc/$GNU_VERSION
module load lib/hwloc/2.1.0
module load openmpi/4.1.6-gnu-11.2.0-cuda-12.0
module load nvidia/cuda/$CUDA_VERSION

BUILD_DIR=_build/cuda-$CUDA_VERSION-gnu-$GNU_VERSION
INSTALL_DIR=$HOME/local/yakl-cuda-$CUDA_VERSION-gnu-$GNU_VERSION

mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -DYAKL_ARCH=CUDA -DYAKL_HAVE_MPI=ON -DYAKL_MANAGED_MEMORY=ON -DCMAKE_CUDA_ARCHITECTURES="$CUDA_ARCH" -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR ../..

make
make install
```
