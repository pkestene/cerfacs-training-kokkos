/*
//@HEADER
// ************************************************************************
//
//                        Kokkos v. 2.0
//              Copyright (2014) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact  H. Carter Edwards (hcedwar@sandia.gov)
//
// ************************************************************************
//@HEADER
*/

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/time.h>

// Include Kokkos Headers
#include <Kokkos_Core.hpp>

#ifdef KOKKOS_ENABLE_CUDA
#  include "CudaTimer.h"
using Timer = CudaTimer;
#else
#  include "HostTimer.h"
using Timer = HostTimer;
#endif

#ifdef USE_FLOAT
using real_t = float;
#else
using real_t = double;
#endif

// =========================================================================
// =========================================================================
/**
 * Compute saxpy n times and measure performance.
 *
 * \param[in] length is the array length
 * \param[in] nrepeat is the number of times computation is done (to improve time measurement
 *            accuracy)
 */
template <typename device_t>
void
saxpy_test(int64_t length, int nrepeat)
{
  using exec_space = typename device_t::execution_space;


  // Allocate Views
  Kokkos::View<double *, device_t> x("X", length);
  Kokkos::View<double *, device_t> y("Y", length);
  real_t                           a = 2.0;

  // Initialize arrays
  Kokkos::parallel_for("init arrays",
    Kokkos::RangePolicy<exec_space>(0, length), KOKKOS_LAMBDA(const int & i) {
      x(i) = 1.0 * i;
      y(i) = 3.0 * i;
    });

  // Time saxpy computation
  Timer timer;
  timer.start();
  for (int k = 0; k < nrepeat; ++k)
  {
    // Do saxpy
      Kokkos::parallel_for("compute saxpy",
                           Kokkos::RangePolicy<exec_space>(0, length),
                           KOKKOS_LAMBDA(const int & i) { y(i) = a * x(i) + y(i); });
  }
  timer.stop();

  // Print results
  double time = timer.elapsed();

  if (sizeof(real_t) == sizeof(float))
    printf("Using data type: float\n");
  else
    printf("Using data type: double\n");

  printf("VectorLength    Time(s)    TimePerIterations(s)    size(MB)    BW(GB/s)\n");
  printf("%11li    %2.3lf      %2.3e               %4.3lf     %4.3lf\n",
         length,
         time,
         time / nrepeat,
         1.0e-6 * length * 3 * sizeof(real_t),
         1.0e-9 * length * 3 * nrepeat * sizeof(real_t) / time);

} // saxpy_test

// =========================================================================
// =========================================================================
// =========================================================================
int
main(int argc, char * argv[])
{
  // initialize Kokkos
  Kokkos::initialize(argc, argv);

  // Parameters
  int64_t length = 10000000;  // length of vectors
  int     nrepeat = 100;      // number of integration invocations
  bool    use_device = false; // should we use device or host parallel execution space ?

  // Read command line arguments
  for (int i = 0; i < argc; i++)
  {
    if (strcmp(argv[i], "-l") == 0)
    {
      length = std::atol(argv[++i]);
    }
    else if (strcmp(argv[i], "-nrepeat") == 0)
    {
      nrepeat = std::atoi(argv[++i]);
    }
    else if (strcmp(argv[i], "-use_device") == 0)
    {
      use_device = true;
    }
    else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "-help") == 0))
    {
      printf("SAXPY Options:\n");
      printf("  -l <int>:         length of vectors (default: 10000000)\n");
      printf("  -nrepeat <int>:   number of integration invocations (default: 100)\n");
      printf("  -use_device:      use Kokkos::DefaultExecutionSpace "
             "                    (default: false, i.e. meaning use "
             "                     Kokkos::DefaultHostExecutionSpace)\n");
      printf("  -help (-h):       print this message\n");
      exit(EXIT_SUCCESS);
    }
  }

  using DefaultDevice =
    Kokkos::Device<Kokkos::DefaultExecutionSpace, Kokkos::DefaultExecutionSpace::memory_space>;

  using DefaultHostDevice = Kokkos::Device<Kokkos::DefaultHostExecutionSpace,
                                           Kokkos::DefaultHostExecutionSpace::memory_space>;

  if (use_device)
  {
    printf("Using exec_space=%s, mem_space=%s\n",
           DefaultDevice::execution_space::name(),
           DefaultDevice::memory_space::name());
    saxpy_test<DefaultDevice>(length, nrepeat);
  }
  else
  {
    printf("Using exec_space=%s, mem_space=%s\n",
           DefaultHostDevice::execution_space::name(),
           DefaultHostDevice::memory_space::name());
    saxpy_test<DefaultHostDevice>(length, nrepeat);
  }

  // shutdown Kokkos
  Kokkos::finalize();

  return EXIT_SUCCESS;
}
