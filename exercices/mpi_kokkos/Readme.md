0. MPI environment

```shell
module load openmpi/4.1.6-gnu-11.2.0-cuda-12.0
```

1. Build for CPU (Kokkos::OpenMP) on kraken

```shell
# if using OpenMP as default backend
module load kokkos/4.1.00-openmp-gnu-11.2.00-RelWithDebInfo
mkdir -p _build/openmp
cd _build/openmp
cmake ../..
make
```

2. Build for GPU (Kokkos::Cuda) on kraken

```shell
# if using Cuda as default backend
module load kokkos/4.1.00-cuda-12.0-gnu-11.2.00-RelWithDebInfo
mkdir -p _build/cuda
cd _build/cuda
cmake ../..
make
```

This example is the most simple thing you think of:
just initialize MPI, then Kokkos, and finaly check
how MPI ranks and GPU are binded.

example: on a node with 2 GPUs
mpirun -np 2 ./test_mpi.cpu --ndevices=2
you should see the correct binding

If you omit option "--ndevices=2", all MPI tasks will use the same GPU (#0)
