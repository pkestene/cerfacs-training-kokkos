#include <cmath>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <iostream>

#include <string>
#include <vector>

#include "common.h"
#include "params.h"
#include "DataContext.h"
#include "DataContextKokkos.h"

#include "HostTimer.h"
#ifdef KOKKOS_ENABLE_CUDA
#  include "CudaTimer.h"
#endif // KOKKOS_ENABLE_CUDA

#ifdef KOKKOS_ENABLE_CUDA
using Timer = CudaTimer;
#else
using Timer = HostTimer;
#endif

#include "laplace2d_serial_kernel.h"
#include "laplace2d_kokkos_kernel.h"

#include "check_results.h"

namespace laplace
{

#include <mpi.h>

// ========================================================================
// ========================================================================
void
test(int NX, int NY, int iter_max)
{

  int rank, nRanks;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

  /*
   * Inititalize some parameters.
   */

#ifdef USE_DOUBLE
  real_t tol = 1e-5;
#else
  real_t tol = 1e-4;
#endif

  real_t runtime_serial = 0;
  real_t runtime_kokkos = 0;

#ifdef USE_DOUBLE
  if (rank == 0)
    printf("DOUBLE PRECISION computations\n");
#else
  if (rank == 0)
    printf("SINGLE PRECISION computations\n");
#endif

  /*
   * Perform serial reference computation.
   */
  if (rank == 0)
    printf("Jacobi relaxation Calculation: %d x %d mesh (serial on rank 0)\n", NY, NX);

  // make sure we have correct bounds (for MPI version only)
  // ix_start/ix_end/iy_start/iy_end not used for serial computations
  int ix_start = 1;
  int ix_end = (NX - 1);

  // Ensure correctness if NY%nRanks != 0
  int chunk_size = ceil((1.0 * NY) / nRanks);

  int iy_start = rank * chunk_size;
  int iy_end = iy_start + chunk_size;

  // Do not process boundaries
  iy_start = std::max(iy_start, 1);
  iy_end = std::min(iy_end, NY - 1);

  Params params(NX, NY, iter_max, tol, ix_start, ix_end, iy_start, iy_end);

  // allocate data context for serial computation
  DataContext context(params);

  real_t * rhs = context.rhs;

  // set rhs
  for (int iy = 1; iy < NY - 1; iy++)
  {
    for (int ix = 1; ix < NX - 1; ix++)
    {
      const real_t x = -1.0 + (2.0 * ix / (NX - 1));
      const real_t y = -1.0 + (2.0 * iy / (NY - 1));
      rhs[iy * NX + ix] = exp(-10.0 * (x * x + y * y));
    }
  }

  // serial computation
  printf("Calculate reference solution and time serial execution.\n");
  HostTimer timer;
  timer.start();
  poisson2d_serial(context, params);
  timer.stop();
  runtime_serial = timer.elapsed();


  /*
   * Now do parallel computation with
   * - MPI for distributed paralellism
   * - Kokkos for node-level parallelism
   */

  // Wait for all processes to ensure correct timing of the parallel version
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0)
    printf("Jacobi relaxation Calculation: %d x %d mesh (MPI+Kokkos)\n", NY, NX);

  // kokkos computation context
  DataContextKokkos context_kokkos(params);

  auto A = context_kokkos.A;
  auto Anew = context_kokkos.Anew;
  auto rhs_k = context_kokkos.rhs;

  // initialize context
  Kokkos::deep_copy(A, 0);
  Kokkos::deep_copy(Anew, 0);

  Kokkos::parallel_for(
    Kokkos::MDRangePolicy<Kokkos::Rank<2>>({ 0, 0 }, { NX, NY }),
    KOKKOS_LAMBDA(const int ix, const int iy) {
      const real_t x = -1.0 + (2.0 * ix / (NX - 1));
      const real_t y = -1.0 + (2.0 * iy / (NY - 1));
      rhs_k(ix, iy) = exp(-10.0 * (x * x + y * y));
    });

  Timer timer2;
  timer2.start();
  poisson2d_kokkos(context_kokkos, params);
  timer2.stop();
  runtime_kokkos = timer2.elapsed();

  check_results(context, context_kokkos, params);

  if (rank == 0)
  {
    printf("serial %dx%d: %8.4f secondes\n", NX, NY, runtime_serial);
    printf("kokkos %dx%d: %8.4f secondes\n", NX, NY, runtime_kokkos);
  }

} // test

} // namespace laplace

// ========================================================================
// ========================================================================
int
main(int argc, char * argv[])
{

  /* Init MPI */
  MPI_Init(&argc, &argv);

  /*
   * Initialize kokkos (host + device)
   */
  // this regular initialization: let Kokkos use hwloc to decide
  // - how many OpenMP threads ? Default is to use all available core on node
  // - how GPU and MPI tasks are mapped
  Kokkos::initialize(argc, argv);

  int rank, nRanks;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

  if (rank == 0)
  {
    std::cout << "##########################\n";
    std::cout << "KOKKOS CONFIG             \n";
    std::cout << "##########################\n";

    std::ostringstream msg;
    std::cout << "Kokkos configuration" << std::endl;
    if (Kokkos::hwloc::available())
    {
      msg << "hwloc( NUMA[" << Kokkos::hwloc::get_available_numa_count() << "] x CORE["
          << Kokkos::hwloc::get_available_cores_per_numa() << "] x HT["
          << Kokkos::hwloc::get_available_threads_per_core() << "] )" << std::endl;
    }
    Kokkos::print_configuration(msg);
    std::cout << msg.str();
    std::cout << "##########################\n";
  }

#ifdef KOKKOS_ENABLE_CUDA
  /*
   * Just checking, which MPI rank mapped with which GPU ?
   */
  int cudaDeviceId;
  cudaGetDevice(&cudaDeviceId);
  std::cout << "I'm MPI task #" << rank << " pinned to GPU #" << cudaDeviceId << "\n";
#endif // KOKKOS_ENABLE_CUDA

  // parameters
  int NX = 512;
  int NY = 512;
  int iter_max = 1000;

  std::vector<std::string> args(argv, argv + argc);

  // read command line arguments
  for (unsigned int i = 1; i < args.size(); ++i)
  {
    if (args[i] == "-s" or args[i] == "--size")
    {
      NX = std::stoi(args[i + 1]);
      NY = NX;
    }
    else if (args[i] == "-i" or args[i] == "--iter")
    {
      iter_max = std::stoi(args[i + 1]);
    }
    else if (args[i] == "-h" or args[i] == "--help")
    {
      printf("Laplace options:\n");
      printf("  --size (-s) <int>:   linear image size (default: 512)\n");
      printf("  --iter (-i) <int>:   nax number of iterations (default: 1000)\n");
      printf("  --help (-h)      :   print this message\n");
      exit(EXIT_SUCCESS);
    }
  }

  laplace::test(NX, NY, iter_max);

  Kokkos::finalize();

  MPI_Finalize();

  return EXIT_SUCCESS;

} // main
