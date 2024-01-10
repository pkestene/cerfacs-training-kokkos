# Activity

Just edit these files
- main.cpp
- mandelbrot.h

and fill the blanks marked with /* TODO */

Additionnal question:
- the Mandelbrot computed with Kokkos::Cuda is flipped compared to the one computed with Kokkos::OpenMP. Can you modify the Cuda version, so screen version are the same ? Without performance drop ?

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

# Performance

Use top-level job.sh to run application on kraken

With default parameters (image of size 8192x8192), some performance of the basic version:
 - Nvidia A30 : 0.2 seconds
 - Intel Xeon (g++ 11.2)
   -  8  threads : 100.3 seconds
   - 16  threads : 55.8 seconds
   - 32  threads : 28.0 seconds
