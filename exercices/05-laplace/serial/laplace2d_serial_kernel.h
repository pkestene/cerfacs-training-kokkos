#ifndef LAPLACE2D_KERNEL_H_
#define LAPLACE2D_KERNEL_H_

#include "params.h"
#include "DataContext.h"

/**
 * Compute a reference solution stored in Aref.
 */
void
poisson2d_serial(DataContext & context, Params const & params)
{
  auto iter_max = params.iter_max;
  auto tol = params.tol;

  auto NX = params.NX;
  auto NY = params.NY;

  real_t * Aref = context.Aref;
  real_t * Anew = context.Anew;
  real_t * rhs = context.rhs;

  int    iter = 0;
  real_t error = 1.0;

  while (error > tol && iter < iter_max)
  {
    error = 0.0;

    for (int iy = 1; iy < NY - 1; iy++)
    {
      for (int ix = 1; ix < NX - 1; ix++)
      {

        Anew[iy * NX + ix] =
          -0.25 * (rhs[iy * NX + ix] - (Aref[iy * NX + ix + 1] + Aref[iy * NX + ix - 1] +
                                        Aref[(iy - 1) * NX + ix] + Aref[(iy + 1) * NX + ix]));

        error = fmaxr(error, fabsr(Anew[iy * NX + ix] - Aref[iy * NX + ix]));
      }
    }

    // copy Anew into Aref
    for (int iy = 1; iy < NY - 1; ++iy)
    {
      for (int ix = 1; ix < NX - 1; ++ix)
      {
        Aref[iy * NX + ix] = Anew[iy * NX + ix];
      }
    }

    // Ensure periodic boundary conditions
    for (int ix = 1; ix < NX - 1; ++ix)
    {
      Aref[0 * NX + ix] = Aref[(NY - 2) * NX + ix];
      Aref[(NY - 1) * NX + ix] = Aref[1 * NX + ix];
    }

    for (int iy = 1; iy < NY - 1; ++iy)
    {
      Aref[iy * NX + 0] = Aref[iy * NX + (NX - 2)];
      Aref[iy * NX + (NX - 1)] = Aref[iy * NX + 1];
    }

    if ((iter % 100) == 0)
      printf("%5d, %0.6f\n", iter, error);
    iter++;

  } // end while

} // poisson2d_serial

#endif // LAPLACE2D_KERNEL_H_
