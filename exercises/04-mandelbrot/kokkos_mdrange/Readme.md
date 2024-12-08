# Activity

Just edit these files
- main.cpp
- mandelbrot.h

and fill the blanks marked with /* TODO */

Additionnal question:
- the Mandelbrot computed with Kokkos::Cuda is flipped compared to the one computed with Kokkos::OpenMP. Can you modify the Cuda version, so screen version are the same ? Without performance drop ?

# Minimal cmake information for Kokkos::OpenMP backend on calypso

If you build using kokkos modulefile:
```shell
module load kokkos/4.4.01-openmp-gnu-12.3.0-RelWithDebInfo
mkdir -p _build/openmp; cd _build/openmp
cmake ../..
make -j 8
```

If you build both kokkos and user application:
```shell
mkdir -p _build/openmp; cd _build/openmp
cmake -DMANDELBROT_KOKKOS_BUILD=ON -DMANDELBROT_KOKKOS_BACKEND=OpenMP ../..
make -j 8
```

# Minimal cmake information for Kokkos::Cuda backend on calypso

If you build using kokkos modulefile:
```shell
module load kokkos/4.4.01-cuda-12.4-gnu-12.3.0-RelWithDebInfo
mkdir -p _build/cuda; cd _build/cuda
cmake ../..
make -j 8
```

If you build both kokkos and user application:
```shell
mkdir -p _build/cuda; cd _build/cuda
cmake -DMANDELBROT_KOKKOS_BUILD=ON -DMANDELBROT_KOKKOS_BACKEND=Cuda -DKokkos_ARCH_HOPPER90=ON ../..
make -j 8
```

# Performance

Use top-level job.sh to run application on `calypso`.


Some example performance measurements obtained on `calypso-grace01`.
With default parameters (image of size 8192x8192), some performance of the basic version:
 - Nvidia GPU Hopper : 0.0045 seconds
 - Nvidia CPU Grace (g++ 12.3) :
   -  1  threads : 19.1 seconds
   -  8  threads : 2.4 seconds
   - 16  threads : 1.2 seconds
   - 36  threads : 0.55 seconds
   - 72  threads : 0.29 seconds


Some example performance measurements obtained on `kraken`.
With default parameters (image of size 8192x8192), some performance of the basic version:
 - Nvidia A30 : 0.2 seconds
 - Intel Xeon (g++ 11.2)
   -  8  threads : 100.3 seconds
   - 16  threads : 55.8 seconds
   - 32  threads : 28.0 seconds
