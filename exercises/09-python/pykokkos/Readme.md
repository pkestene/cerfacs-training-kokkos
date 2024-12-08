# Build instructions for kraken

## Requirements

### Modulefiles

minimal env is:
```shell
> module list
Currently Loaded Modulefiles:
  1) gcc/12.3.0_arm   2) tools/cmake/3.29.3_arm    3) nvidia/cuda/12.4      4) python/gloenv3.12_arm
```

### Install miniforge/miniconda

**you don't need that on calypso but on your system you may**

> #### note
>
> miniforge is miniconda with conda-forge as default channel


```shell
# use this on your own machine
mkdir -p ~/miniforge3
wget https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-Linux
bash ~/miniforge3/miniforge.sh -b -u -p ~/miniforge3
rm -rf ~/miniforge3/miniforge.sh
~/miniforge3/bin/conda init bash

# on calypso, just do
module load python/gloenv3.12_arm
```

Logout and reconnect to calypso, so that the newly python env is loaded.

### install pykokkos-base

Slightly adapted from PyKokkos [Readme](https://github.com/kokkos/pykokkos#readme).

```shell
git clone https://github.com/kokkos/pykokkos-base.git
cd pykokkos-base
conda create --name pyk --file requirements.txt python=3.11
conda activate pyk

# - set explicitely the GPU architecture option
# - use regular generated Makefile instead of ninja (default)
# - limit the number of threads to compile
python setup.py install -- -DENABLE_LAYOUTS=ON -DENABLE_MEMORY_TRAITS=OFF -DENABLE_VIEW_RANKS=3 -DENABLE_CUDA=ON -DENABLE_THREADS=OFF -DENABLE_OPENMP=ON -DKokkos_ARCH_HOPPER90=ON -G "Unix Makefiles" -- -j 8
```

!!WARNING!!: compilation can be very very long.

### install pykokkos

```shell
cd ..
git clone https://github.com/kokkos/pykokkos.git
cd pykokkos/
conda install -c conda-forge pybind11 cupy patchelf
pip install --user -e .
```
