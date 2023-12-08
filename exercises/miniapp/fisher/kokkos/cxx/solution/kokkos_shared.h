#ifndef KOKKOS_SHARED_H_
#define KOKKOS_SHARED_H_

#include <Kokkos_Core.hpp>

using Device = Kokkos::DefaultExecutionSpace;

// Alias name for 1d and 2d array (Kokkos view)
using Field1d = Kokkos::View<double *, Device>;
using Field1dHost = Field1d::HostMirror;

using Field2d = Kokkos::View<double **, Device>;
using Field2dHost = Field2d::HostMirror;

// Memory layout typedef
using array_layout = typename Field2d::array_layout;

/**
 * Retrieve cartesian coordiante from index, using memory layout information.
 */
KOKKOS_INLINE_FUNCTION
void
index2coord(int index, int & i, int & j, int Nx, int Ny)
{
  if (std::is_same<array_layout, Kokkos::LayoutLeft>::value)
  { // Left <-> CUDA
    j = index / Nx;
    i = index - j * Nx;
  }
  else
  { // Right <-> OpenMP
    i = index / Ny;
    j = index - i * Ny;
  }
}


#endif // KOKKOS_SHARED_H_
