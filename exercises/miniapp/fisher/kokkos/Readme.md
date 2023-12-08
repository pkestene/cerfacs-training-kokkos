There are 2 variant of kokkos port.
Each of them can target either OpenMP or CUDA backend.
To build you only need to set env variable KOKKOS_PATH
to kokkos install directory.

OpenMP backend is not very efficient on small domain size, but is ok
for large domain (i.e. 1024x1024).
There is room for improvement.

version 1: (cxx)
Use both 1d and 2d Kokkos views.

version 2: (cxx_v2)
Same as version 1, but uses the new multi-dimensional range policy.
