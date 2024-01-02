# Build instructions for kraken

## Requirements

### Modulefiles

minimal env is:
```shell
> module list
Currently Loaded Modulefiles:
  1) cmake/3.25.0    2) cuda/11.8
```

### Install miniconda

```shell
mkdir -p $HOME/install/miniconda
cd $HOME/install/miniconda
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
bash ./Miniconda3-latest-Linux-x86_64.sh -b -u -p  $HOME/install/miniconda
./bin/conda init bash
```

Logout and reconnect to kraken, so that the newly python env is loaded.

### install pykokkos-base

Slightly adapted from PyKokkos [Readme](https://github.com/kokkos/pykokkos#readme).

```shell
git clone https://github.com/kokkos/pykokkos-base.git
cd pykokkos-base
conda create --name pykokkos --file requirements.txt
conda activate pykokkos

# - set explicitely the GPU architecture option
# - limit the number of threads to compile
python setup.py install -- -DENABLE_LAYOUTS=ON -DENABLE_MEMORY_TRAITS=OFF -DENABLE_VIEW_RANKS=3 -DENABLE_CUDA=ON -DENABLE_THREADS=OFF -DENABLE_OPENMP=ON -DKokkos_ARCH_AMPERE80=ON -G "Unix Makefiles" -- -j 4
```

!!WARNING!!: compilation can be very very long (more than an hour on hpcai).

### install pykokkos

```shell
cd ..
git clone https://github.com/kokkos/pykokkos.git
cd pykokkos/
conda install -c conda-forge pybind11 cupy patchelf
pip install --user -e .
```
