# Purpose

Illustrate the use of partition spaces; see what performance advantages it can bring compare to kokkos_range version studied earlier

# Activity

- no code to write.
- open src/main.cpp see how partition space works; asks questions if the code is unclear.
- build and run the cuda version
- use Nvidia `nsys` profiling tools to retrieve trace files

```shell
nsys profile --stats=true --trace=cuda ./mandelbrot_kokkos_pipeline0 -s 8192
nsys-ui report1.nsys-rep
```
- do the same for `./mandelbrot_kokkos_range`
- what differences do you see ? Can you explain them ?
- change parameter `MAX_ITERS`; what is the effect on computation on both version ?


# Minimal cmake information for Kokkos::OpenMP/Cuda backends on kraken

If you build using kokkos modulefile:
```shell
module load kokkos/4.1.00-openmp-gnu-11.2.0-RelWithDebInfo
mkdir -p _build/openmp; cd _build/openmp
cmake ../..
make -j 8
```

# Minimal cmake information for Kokkos::Cuda backend on kraken

```shell
module load kokkos/4.1.00-cuda-12.0-gnu-11.2.0-RelWithDebInfo
mkdir _build/cuda; cd _build/cuda
cmake ../..
make -j 8
```

If you build both kokkos and user application:
```shell
mkdir -p _build/openmp; cd _build/openmp
cmake -DMANDELBROT_KOKKOS_BUILD=ON -DMANDELBROT_KOKKOS_BACKEND=OpenMP -DKokkos_ARCH_SKX=ON ../..
make -j 8
```

# Minimal cmake information for Kokkos::Cuda backend on kraken

```shell
mkdir build_cuda; cd build_cuda
cmake -DMANDELBROT_KOKKOS_BUILD=ON -DMANDELBROT_KOKKOS_BACKEND=Cuda -DKokkos_ARCH_AMPERE80=ON ../..
make -j 8
```
