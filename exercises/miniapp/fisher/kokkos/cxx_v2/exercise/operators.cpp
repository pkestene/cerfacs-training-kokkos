//******************************************
// operators.f90
// based on min-app code written by Oliver Fuhrer, MeteoSwiss
// modified by Ben Cumming, CSCS
// *****************************************

// Description: Contains simple operators which can be used on 3d-meshes

#include "data.h"
#include "operators.h"
#include "stats.h"

namespace operators
{

void
diffusion(Field2d U, Field2d S, const DataWarehouse & dw)
{

  using data::options;

  const Field1d & bndE = dw.bndE;
  const Field1d & bndW = dw.bndW;
  const Field1d & bndN = dw.bndN;
  const Field1d & bndS = dw.bndS;

  const Field2d & x_old = dw.x_old;

  double dxs = 1000. * (options.dx * options.dx);
  double alpha = options.alpha;
  int    nx = options.nx;
  int    ny = options.ny;
  int    iend = nx - 1;
  int    jend = ny - 1;

  // the interior grid points
  /* TODO */
  /*
    for (int j=1; j < jend; j++) {
        for (int i=1; i < iend; i++) {
            S(i,j) = -(4. + alpha) * U(i,j)               // central point
                                    + U(i-1,j) + U(i+1,j) // east and west
                                    + U(i,j-1) + U(i,j+1) // north and south
                                    + alpha * x_old(i,j)
                                    + dxs * U(i,j) * (1.0 - U(i,j));
        }
    }
  */

  // the east boundary
  /* TODO */
  /*    {
        int i = nx - 1;
        for (int j = 1; j < jend; j++)
        {
            S(i,j) = -(4. + alpha) * U(i,j)
                        + U(i-1,j) + U(i,j-1) + U(i,j+1)
                        + alpha*x_old(i,j) + bndE[j]
                        + dxs * U(i,j) * (1.0 - U(i,j));
        }
  } */

  // the west boundary
  /* TODO */
  /* {
    int i = 0;
    for (int j = 1; j < jend; j++)
      {
  S(i,j) = -(4. + alpha) * U(i,j)
    + U(i+1,j) + U(i,j-1) + U(i,j+1)
    + alpha * x_old(i,j) + bndW[j]
    + dxs * U(i,j) * (1.0 - U(i,j));
      }
      } */

  // the north boundary (plus NE and NW corners)
  {
    int j = ny - 1;


    // north boundary
    // for (int i = 1; i < iend; i++)
    Kokkos::parallel_for(
      "border_north", Kokkos::RangePolicy<>(0, nx), KOKKOS_LAMBDA(int index) {
        if (index == 0)
        {
          int i = 0; // NW corner
          S(i, j) = -(4. + alpha) * U(i, j) + U(i + 1, j) + U(i, j - 1) + alpha * x_old(i, j) +
                    bndW[j] + bndN[i] + dxs * U(i, j) * (1.0 - U(i, j));
        }
        else if (index == nx - 1)
        {
          int i = nx - 1; // NE corner
          S(i, j) = -(4. + alpha) * U(i, j) + U(i - 1, j) + U(i, j - 1) + alpha * x_old(i, j) +
                    bndE[j] + bndN[i] + dxs * U(i, j) * (1.0 - U(i, j));
        }
        else
        {
          int i = index;
          S(i, j) = -(4. + alpha) * U(i, j) + U(i - 1, j) + U(i + 1, j) + U(i, j - 1) +
                    alpha * x_old(i, j) + bndN[i] + dxs * U(i, j) * (1.0 - U(i, j));
        }
      });
  }

  // the south boundary
  {
    int j = 0;

    // south boundary
    // for (int i = 1; i < iend; i++)
    Kokkos::parallel_for(
      "border_south", Kokkos::RangePolicy<>(0, nx), KOKKOS_LAMBDA(int index) {
        if (index == 0)
        {
          int i = 0; // SW corner
          S(i, j) = -(4. + alpha) * U(i, j) + U(i + 1, j) + U(i, j + 1) + alpha * x_old(i, j) +
                    bndW[j] + bndS[i] + dxs * U(i, j) * (1.0 - U(i, j));
        }
        else if (index == nx - 1)
        {
          int i = nx - 1; // SE corner
          S(i, j) = -(4. + alpha) * U(i, j) + U(i - 1, j) + U(i, j + 1) + alpha * x_old(i, j) +
                    bndE[j] + bndS[i] + dxs * U(i, j) * (1.0 - U(i, j));
        }
        else
        {
          int i = index;
          S(i, j) = -(4. + alpha) * U(i, j) + U(i - 1, j) + U(i + 1, j) + U(i, j + 1) +
                    alpha * x_old(i, j) + bndS[i] + dxs * U(i, j) * (1.0 - U(i, j));
        }
      });
  }

  // Accumulate the flop counts
  // 8 ops total per point
  stats::flops_diff += +12 * (nx - 2) * (ny - 2) // interior points
                       + 11 * (nx - 2 + ny - 2)  // NESW boundary points
                       + 11 * 4;                 // corner points
}

} // namespace operators
