#ifndef CHECK_RESULTS_H_
#define CHECK_RESULTS_H_

#include "DataContext.h"
#include "DataContextKokkos.h"

namespace laplace
{

int
check_results(DataContext & context_serial, DataContextKokkos & context_kokkos, Params & params)

{

  int    NX = params.NX;
  int    NY = params.NY;
  real_t tol = params.tol;

  int ix_start = 1;    // context_kokkos.ix_start;
  int ix_end = NX - 1; // context_kokkos.ix_end;
  int iy_start = 1;    // context_kokkos.iy_start;
  int iy_end = NY - 1; // context_kokkos.iy_end;

  // reference results
  real_t * Aref = context_serial.Aref;

  // get a host copy of kokkos computation
  auto A_h = Kokkos::create_mirror_view_and_copy(Kokkos::HostSpace{}, context_kokkos.A);

  // now check results
  int result_correct = 1;
  int error_count = 0;
  for (int iy = iy_start; iy < iy_end /*&& (result_correct == 1)*/; iy++)
  {

    for (int ix = ix_start; ix < ix_end /*&& (result_correct == 1)*/; ix++)
    {

      if (fabs(Aref[iy * NX + ix] - A_h(ix, iy)) >= tol)
      {
        fprintf(stderr,
                "ERROR: A[%d][%d] = %f does not match %f (reference) --- abs(delta)=%f\n",
                iy,
                ix,
                A_h(ix, iy),
                Aref[iy * NX + ix],
                fabs(A_h(ix, iy) - Aref[iy * NX + ix]));
        result_correct = 0;
        error_count++;
      }
    }
  }

  if (error_count)
    printf("Found %d errors\n", error_count);

  return result_correct;

} // check_results

} // namespace laplace

#endif // CHECK_RESULTS
