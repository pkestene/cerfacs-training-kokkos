#include <cmath>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cassert>

#include <string>
#include <vector>

#include "common.h"
#include "params.h"
#include "DataContext.h"

#include "HostTimer.h"

#include "laplace2d_serial_kernel.h"

// ========================================================================
// ========================================================================
void
test_laplace(int NX, int NY, int iter_max)
{

  real_t tol = 1e-5;
  Params params(NX, NY, iter_max, tol);

  // allocate data context
  DataContext context(params);

  real_t * rhs = context.rhs;

  // set rhs
  for (int iy = 1; iy < NY - 1; iy++)
  {
    for (int ix = 1; ix < NX - 1; ix++)
    {
      const real_t x = -1.0 + (2.0 * ix / (NX - 1));
      const real_t y = -1.0 + (2.0 * iy / (NY - 1));
      rhs[iy * NX + ix] = expr(-10.0 * (x * x + y * y));
    }
  }

  // serial computation
  HostTimer timer;
  timer.start();
  poisson2d_serial(context, params);
  timer.stop();
  real_t runtime_serial = timer.elapsed();

  printf("%dx%d: %8.4f secondes\n", NX, NY, runtime_serial);

} // test_laplace

// ========================================================================
// ========================================================================
// ========================================================================
int
main(int argc, char * argv[])
{

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

  test_laplace(NX, NY, iter_max);

  return EXIT_SUCCESS;

} // main
