#ifndef COMMON_H_
#define COMMON_H_

#include <Kokkos_Macros.hpp>
#include <Kokkos_Core.hpp>
#include <Kokkos_MathematicalConstants.hpp>
#include <Kokkos_MathematicalFunctions.hpp>

namespace laplace
{

#ifdef USE_DOUBLE
using real_t = double;
#  define MPI_REAL_TYPE MPI_DOUBLE
#else
using real_t = float;
#  define MPI_REAL_TYPE MPI_FLOAT
#endif

// math function
#if KOKKOS_VERSION_MAJOR > 3
using Kokkos::exp;
using Kokkos::fmax;
using Kokkos::fabs;
#else
using Kokkos::Experimental::exp;
using Kokkos::Experimental::fmax;
using Kokkos::Experimental::fabs;
#endif

} // namespace laplace

#endif // COMMON_H_
