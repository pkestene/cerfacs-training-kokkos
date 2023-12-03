#ifndef CHECK_RESULTS_H_
#define CHECK_RESULTS_H_

#include "DataContext.h"
#include "DataContextKokkos.h"

#include <mpi.h>

namespace laplace
{

int
check_results(DataContext & context_serial, DataContextKokkos & context_kokkos, Params & params)

{

  int    NX = params.NX;
  int    NY = params.NY;
  real_t tol = params.tol;

  int ix_start = params.ix_start;
  int ix_end = params.ix_end;
  int iy_start = params.iy_start;
  int iy_end = params.iy_end;

  int mpi_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

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
                "[rank=%d] ERROR: A[%d][%d] = %f does not match %f (reference) --- abs(delta)=%f\n",
                mpi_rank,
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
    printf("[rank=%d] Found %d errors\n", mpi_rank, error_count);

  // perform MPI reduction on errors
  int global_result_correct = 0;
  MPI_Allreduce(&result_correct, &global_result_correct, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
  result_correct = global_result_correct;

  if (mpi_rank == 0)
  {
    if (result_correct)
      printf("Serial and Kokkos results match !\n");
    else
      printf("Serial and Kokkos results don't match !\n");
  }

  return result_correct;
} // check_results

} // namespace laplace

#endif // CHECK_RESULTS
