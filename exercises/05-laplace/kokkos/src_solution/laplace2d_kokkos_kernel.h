#ifndef LAPLACE2D_KOKKOS_KERNEL_H_
#define LAPLACE2D_KOKKOS_KERNEL_H_

#include "params.h"
#include "DataContextKokkos.h"

namespace laplace
{

/**
 * Compute a kokkos solution stored in Anew.
 */
void
poisson2d_kokkos(DataContextKokkos & context, Params & params)
{
  const auto iter_max = params.iter_max;
  const auto tol = params.tol;

  const auto NX = params.NX;
  const auto NY = params.NY;

  auto A = context.A;
  auto Anew = context.Anew;
  auto rhs = context.rhs;

  int    iter = 0;
  real_t error = 1.0;

  while (error > tol && iter < iter_max)
  {
    error = 0.0;

    Kokkos::parallel_reduce(
      Kokkos::MDRangePolicy<Kokkos::Rank<2>>({ 0, 0 }, { NX, NY }),
      KOKKOS_LAMBDA(const int i, const int j, real_t & update_error) {
        const int NX = params.NX;
        const int NY = params.NY;

        if (j >= 1 and j < NY - 1 and i >= 1 and i < NX - 1)
        {

          Anew(i, j) =
            -0.25 * (rhs(i, j) - (A(i + 1, j) + A(i - 1, j) + A(i, j + 1) + A(i, j - 1)));

          update_error = fmax(update_error, fabs(Anew(i, j) - A(i, j)));
        }
      },
      Kokkos::Max<real_t>(error));

    // copy Anew into A (bulk)
    // Kokkos::deep_copy(context.A, context.Anew);
    Kokkos::parallel_for(
      Kokkos::MDRangePolicy<Kokkos::Rank<2>>({ 0, 0 }, { NX, NY }),
      KOKKOS_LAMBDA(const int ix, const int iy) {
        if (ix >= 1 and ix < NX - 1 and iy >= 1 and iy < NY - 1)
        {
          A(ix, iy) = Anew(ix, iy);
        }
      });

    // Ensure periodic boundary conditions
    Kokkos::parallel_for(
      Kokkos::RangePolicy<>(0, NX), KOKKOS_LAMBDA(const int ix) {
        if (ix >= 1 and ix < NX - 1)
        {
          A(ix, 0) = A(ix, NY - 2);
          A(ix, NY - 1) = A(ix, 1);
        }
      });

    Kokkos::parallel_for(
      Kokkos::RangePolicy<>(0, NY), KOKKOS_LAMBDA(const int iy) {
        if (iy >= 1 and iy < NY - 1)
        {
          A(0, iy) = A(NX - 2, iy);
          A(NX - 1, iy) = A(1, iy);
        }
      });

    if ((iter % 100) == 0)
      printf("%5d, %0.6f\n", iter, error);
    iter++;

  } // end while

} // poisson2d_kokkos

} // namespace laplace

#endif // LAPLACE2D_KOKKOS_KERNEL_H_
