#include <cmath>
#include <cstring>
#include <cstdio>
#include <cassert>

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

#include "laplace2d_serial_kernel.h"
#include "laplace2d_kokkos_kernel.h"

#include "check_results.h"

namespace laplace
{

// ========================================================================
// ========================================================================
void
test(int NX, int NY, int iter_max)
{

#ifdef USE_DOUBLE
  real_t tol = 1e-5;
#else
  real_t tol = 1e-4;
#endif


#ifdef USE_DOUBLE
  printf("DOUBLE PRECISION computations\n");
#else
  printf("SINGLE PRECISION computations\n");
#endif

  printf("Jacobi relaxation Calculation: %d x %d mesh\n", NY, NX);

  Params params(NX, NY, iter_max, tol);

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
  real_t runtime_serial = timer.elapsed();

  // kokkos computation
  printf("Parallel execution with kokkos.\n");
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

  timer.reset();
  timer.start();
  poisson2d_kokkos(context_kokkos, params);
  timer.stop();
  real_t runtime_kokkos = timer.elapsed();

  if (check_results(context, context_kokkos, params))
    printf("Serial and Kokkos results match !\n");
  else
    printf("Serial and Kokkos results don't match !\n");

  printf("serial %dx%d: %8.4f secondes\n", NX, NY, runtime_serial);
  printf("kokkos %dx%d: %8.4f secondes\n", NX, NY, runtime_kokkos);

} // test

} // namespace laplace

// ========================================================================
// ========================================================================
int
main(int argc, char * argv[])
{

  /*
   * Initialize kokkos (host + device)
   */
  Kokkos::initialize(argc, argv);

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
      printf("\n");
      printf("Laplace options:\n");
      printf("  --size (-s) <int>:   linear image size (default: 512)\n");
      printf("  --iter (-i) <int>:   nax number of iterations (default: 1000)\n");
      printf("  --help (-h)      :   print this message\n");
      exit(EXIT_SUCCESS);
    }
  }

  laplace::test(NX, NY, iter_max);

  Kokkos::finalize();

  return EXIT_SUCCESS;

} // main
