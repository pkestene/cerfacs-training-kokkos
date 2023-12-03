#ifndef LAPLACE2D_KOKKOS_KERNEL_H_
#define LAPLACE2D_KOKKOS_KERNEL_H_

#include "params.h"
#include "DataContextKokkos.h"

#include <mpi.h>

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
  const auto [[maybe_unused]] NY = params.NY;

  auto A = context.A;
  auto Anew = context.Anew;
  auto rhs = context.rhs;

  int    iter = 0;
  real_t error = 1.0;

  int mpi_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  int mpi_ranks;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_ranks);

  int ix_start = params.ix_start;
  int ix_end = params.ix_end;
  int iy_start = params.iy_start;
  int iy_end = params.iy_end;

  // Ensure correctness if NY%nRanks != 0
  int NY_chunk = iy_end - iy_start;

  while (error > tol && iter < iter_max)
  {
    error = 0.0;

    Kokkos::parallel_reduce(
      Kokkos::MDRangePolicy<Kokkos::Rank<2>>({ ix_start, iy_start }, { ix_end, iy_end }),
      KOKKOS_LAMBDA(const int i, const int j, real_t & update_error) {
        Anew(i, j) = -0.25 * (rhs(i, j) - (A(i + 1, j) + A(i - 1, j) + A(i, j + 1) + A(i, j - 1)));

        update_error = fmax(update_error, fabs(Anew(i, j) - A(i, j)));
      },
      Kokkos::Max<real_t>(error));

    // make sure "error" is up to data
    Kokkos::fence();

    // MPI reduce to compute global error
    real_t globalerror = 0.0;
    MPI_Allreduce(&error, &globalerror, 1, MPI_REAL_TYPE, MPI_MAX, MPI_COMM_WORLD);
    error = globalerror;

    // copy Anew into A for current chunk
    // secure borders
    Kokkos::parallel_for(
      Kokkos::RangePolicy<>(0, NX), KOKKOS_LAMBDA(const int ix) {
        if (ix >= ix_start and ix < ix_end)
        {
          A(ix, iy_start) = Anew(ix, iy_start);
          A(ix, iy_end - 1) = Anew(ix, iy_end - 1);
        }
      });

    // TODO/CHECK if we could async copy (to obtain overlap MPI_Sendrevc)
    Kokkos::parallel_for(
      Kokkos::MDRangePolicy<Kokkos::Rank<2>>({ ix_start, iy_start }, { ix_end, iy_end }),
      KOKKOS_LAMBDA(const int ix, const int iy) {
        if (iy >= iy_start + 1 and iy < iy_end - 1)
        {
          A(ix, iy) = Anew(ix, iy);
        }
      });

    // determine with whom we will have to exchange data
    int top = (mpi_rank == 0) ? (mpi_ranks - 1) : mpi_rank - 1;
    int bottom = (mpi_rank == (mpi_ranks - 1)) ? 0 : mpi_rank + 1;

    // assumes a CUDA-aware MPI implementation
    {
      // create border buffer
      DataArray1d sendBuf("sendBuf", NX);
      DataArray1d recvBuf("recvBuf", NX);

      // 1. Sent row iy_start (first modified row) to top
      //    receive lower boundary (iy_end) from bottom
      /*
       * downward communications:
       * - prepare to send
       * - send/recvbuf
       * - postprocess received data
       */

      // copy A's border into rowBlock
      Kokkos::parallel_for(
        Kokkos::RangePolicy<>(0, NX),
        KOKKOS_LAMBDA(const int ix) { sendBuf(ix) = A(ix, iy_start); });
      Kokkos::fence();

      MPI_Sendrecv(sendBuf.data(),
                   NX,
                   MPI_REAL_TYPE,
                   top,
                   0,
                   recvBuf.data(),
                   NX,
                   MPI_REAL_TYPE,
                   bottom,
                   0,
                   MPI_COMM_WORLD,
                   MPI_STATUS_IGNORE);

      // copy back in place received data
      Kokkos::parallel_for(
        Kokkos::RangePolicy<>(0, NX), KOKKOS_LAMBDA(const int ix) { A(ix, iy_end) = recvBuf(ix); });


      // 2. Sent row (iy_end-1) (last modified row) to bottom receive upper boundary (iy_start-1)
      // from top
      /*
       * upward communications:
       * - prepare to send
       * - send/recvbuf
       * - postprocess received data
       */
      Kokkos::parallel_for(
        Kokkos::RangePolicy<>(0, NX),
        KOKKOS_LAMBDA(const int ix) { sendBuf(ix) = A(ix, iy_end - 1); });
      Kokkos::fence();

      MPI_Sendrecv(sendBuf.data(),
                   NX,
                   MPI_REAL_TYPE,
                   bottom,
                   0,
                   recvBuf.data(),
                   NX,
                   MPI_REAL_TYPE,
                   top,
                   0,
                   MPI_COMM_WORLD,
                   MPI_STATUS_IGNORE);


      // copy back in place received data
      Kokkos::parallel_for(
        Kokkos::RangePolicy<>(0, NX),
        KOKKOS_LAMBDA(const int ix) { A(ix, iy_start - 1) = recvBuf(ix); });

    } // end of MPI communications
    Kokkos::fence();

    Kokkos::parallel_for(
      Kokkos::RangePolicy<>(iy_start, iy_end), KOKKOS_LAMBDA(const int iy) {
        A(0, iy) = A(NX - 2, iy);
        A(NX - 1, iy) = A(1, iy);
      });

    if (mpi_rank == 0 and (iter % 100) == 0)
      printf("%5d, %0.6f\n", iter, error);
    iter++;

  } // namespace laplace

} // poisson2d_kokkos

} // namespace laplace

#endif // LAPLACE2D_KOKKOS_KERNEL_H_
