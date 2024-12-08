Reuse material from [Kokkos-tutorial](https://github.com/kokkos/kokkos-tutorials/tree/main/Exercises/kokkoskernels)

# Environment configuration

```shell
# for running on CPU
module load kokkos/4.4.01-openmp-gnu-12.3.0-RelWithDebInfo
module load kokkos-kernels/4.4.01-openmp-gnu-12.3.0-RelWithDebInfo

# for running on GPU
module load nvidia/cuda/12.4
module load kokkos/4.4.01-cuda-12.4-gnu-12.3.0-RelWithDebInfo
module load kokkos-kernels/4.4.01-cuda-12.4-gnu-12.3.0-RelWithDebInfo
```

# Building an exercise

e.g. `InnerProduct`

```shell
cd InnerProduct/Begin

# for running on CPU with Kokkos::OpenMP
mkdir -p _build/openmp
cd _build/openmp
cmake ../..
make -j 8
# then re-use top-level job.sh to submit job to slurm
```
