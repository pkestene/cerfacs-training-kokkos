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
      /* TODO: create a MDRange policy */,
      KOKKOS_LAMBDA(/* TODO: provide lambda interface */) {
        const int NX = params.NX;
        const int NY = params.NY;

        if (j >= 1 and j < NY - 1 and i >= 1 and i < NX - 1)
        {

          Anew(i, j) =
            -0.25 * (rhs(i, j) - (A(i + 1, j) + A(i - 1, j) + A(i, j + 1) + A(i, j - 1)));

          update_error = fmax(update_error, fabs(Anew(i, j) - A(i, j)));
        }
      },
      /* TODO : create a reducer for variable "error" */);

    // copy Anew into A (bulk)
    // Kokkos::deep_copy(context.A, context.Anew);
    Kokkos::parallel_for(
      /* TODO: create a MDRange policy */, KOKKOS_LAMBDA(const int ix, const int iy) {
        if (ix >= 1 and ix < NX - 1 and iy >= 1 and iy < NY - 1)
        {
          A(ix, iy) = Anew(ix, iy);
        }
      });

    // Ensure periodic boundary conditions
    // TODO: add parallel_for for border condition
    // TODO: see laplace2d_serial_kernel


    if ((iter % 100) == 0)
      printf("%5d, %0.6f\n", iter, error);
    iter++;

  } // end while

} // poisson2d_kokkos

} // namespace laplace

#endif // LAPLACE2D_KOKKOS_KERNEL_H_
