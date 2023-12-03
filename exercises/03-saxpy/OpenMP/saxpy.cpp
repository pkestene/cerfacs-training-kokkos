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

#include "HostTimer.h"

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
void
saxpy_test(int64_t length, int nrepeat)
{
  // Allocate arrays
  real_t * x = new real_t[length];
  real_t * y = new real_t[length];
  real_t   a = 2.0;

  // Initialize arrays
#pragma omp parallel for
  for (int i = 0; i < length; ++i)
  {
    x[i] = 1.0 * i;
    y[i] = 3.0 * i;
  }

  // Time saxpy computation
  HostTimer timer;
  timer.start();
  for (int k = 0; k < nrepeat; ++k)
  {
    // Do saxpy
#pragma omp parallel for
#pragma ivdep
    for (int i = 0; i < length; ++i)
    {
      y[i] = a * x[i] + y[i];
    }
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

  delete[] x;
  delete[] y;

} // saxpy_test

// =========================================================================
// =========================================================================
// =========================================================================
int
main(int argc, char * argv[])
{

  // Parameters
  int64_t length = 10000000; // length of vectors
  int     nrepeat = 100;     // number of integration invocations

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
    else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "-help") == 0))
    {
      printf("SAXPY Options:\n");
      printf("  -l <int>:         length of vectors (default: 10000000)\n");
      printf("  -nrepeat <int>:   number of integration invocations (default: 100)\n");
      printf("  -help (-h):       print this message\n");
      exit(EXIT_SUCCESS);
    }
  }

  saxpy_test(length, nrepeat);

  return EXIT_SUCCESS;
}
