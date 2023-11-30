#ifndef KOKKOS_SHARED_H_
#define KOKKOS_SHARED_H_

#include <Kokkos_Core.hpp>
#include <Kokkos_Macros.hpp>

// define type real_t
#include "common.h"

namespace laplace
{

// define Kokkos execution space
using Device = Kokkos::DefaultExecutionSpace;

// Data array for laplace computation
using DataArray = Kokkos::View<real_t **, Device>;

using DataArray1d = Kokkos::View<real_t *, Device>;


/**
 * Retrieve cartesian coordinate from index, using memory layout information.
 *
 * for each execution space define a prefered layout.
 * Prefer left layout  for CUDA execution space.
 * Prefer right layout for OpenMP execution space.
 */
KOKKOS_INLINE_FUNCTION
void
index2coord(int index, int & i, int & j, [[maybe_unused]] int Nx, [[maybe_unused]] int Ny)
{
#ifdef KOKKOS_ENABLE_CUDA
  j = index / Nx;
  i = index - j * Nx;
#else
  i = index / Ny;
  j = index - i * Ny;
#endif
}

KOKKOS_INLINE_FUNCTION
int
coord2index(int i, int j, [[maybe_unused]] int Nx, [[maybe_unused]] int Ny)
{
#ifdef KOKKOS_ENABLE_CUDA
  return i + Nx * j; // left layout
#else
  return j + Ny * i; // right layout
#endif
}

} // namespace laplace

#endif // KOKKOS_SHARED_H_
