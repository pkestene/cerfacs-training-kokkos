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
#else // OpenMP
#  include "HostTimer.h"
using Timer = HostTimer;
#endif

using real_t = double;

using ExecSpace = Kokkos::DefaultExecutionSpace;
using Array1d = Kokkos::View<real_t *, ExecSpace>;

// ===============================================================
// ===============================================================
// ===============================================================
void
test_dotprod(int64_t length, int nrepeat, int nteams)
{

  // TODO1: Allocate Views for array B and C

  // Initialize arrays
  Kokkos::parallel_for(
    Kokkos::RangePolicy<ExecSpace>(0, length), KOKKOS_LAMBDA(const int64_t & i) {
      // B(i) = cos(2*M_PI*i/100.);
      // C(i) = cos(2*M_PI*i/100.);
      B(i) = 1.0 * i;
      C(i) = 1.0;
    });

  // Time computation
  using team_policy = Kokkos::TeamPolicy<ExecSpace, Kokkos::IndexType<int64_t>>;
  using team_member = typename team_policy::member_type;

  const team_policy policy(/* TODO2: How many teams ? */,
                           /* TODO3: How many threads per teams ? */);

  // final result allocation
  Kokkos::View<real_t[1], ExecSpace> sum("sum");
  Kokkos::deep_copy(sum, 0.0);

  Timer timer;
  timer.start();
  for (int k = 0; k < nrepeat; k++)
  {

    // Do dotprod
    Kokkos::parallel_for(
      policy, KOKKOS_LAMBDA(const team_member & member) {
        // inside a team, compute dot product partial result as a parallel reduce operation
        real_t partial_dot_prod = 0;
        int    teamId = member.league_rank();

        // divide computation among the teams
        int64_t nbItems = length / nteams;

        // get the begin/end index for teams
        int64_t begin = /* TODO4 */;
        int64_t end = /* TODO4 */;

        // divide work among threads inside a teams
        Kokkos::parallel_reduce(
          Kokkos::TeamThreadRange(member, begin, end),
          [&](const int64_t & i, real_t & update) { /* TODO5: do the actual computation */ },
          Kokkos::Sum<real_t>(partial_dot_prod));

        // only one thread per team, collect the final reduction result, and write it
        // the output view
        Kokkos::single(Kokkos::PerTeam(member), [&]() {
          /* TODO6 : accumulate partial result in variable sum */
          /* Why do we need to use atomic operation here */
        });
      });
  }
  timer.stop();

  auto sum_host = Kokkos::create_mirror_view(sum);
  Kokkos::deep_copy(sum_host, sum);

  printf("Expected result = %f, actual result %f | error=%f\n",
         1.0 * (length - 1) * length / 2,
         sum_host(0) / nrepeat,
         fabs(sum_host(0) / nrepeat - (length - 1.0) * length / 2));

  // Print results
  double time_seconds = timer.elapsed();

  printf("#VectorLength  Time(s) TimePerIterations(s) size(MB) BW(GB/s)\n");
  printf("%13li %8lf %20.3e  %3.3f %3.3f\n",
         length,
         time_seconds,
         time_seconds / nrepeat,
         1.0e-6 * length * 2 * sizeof(real_t),
         1.0e-9 * length * 2 * nrepeat * sizeof(real_t) / time_seconds);

} // test_dotprod

// ===============================================================
// ===============================================================
// ===============================================================
int
main(int argc, char * argv[])
{

  // Parameters
  int length = 10000000; // length of vectors
  int nrepeat = 10;      // number of integration invocations
  int nteams = 2;        // default number of teams (of threads)

  // Read command line arguments
  for (int i = 0; i < argc; i++)
  {
    if (strcmp(argv[i], "-l") == 0)
    {
      length = atoi(argv[++i]);
    }
    else if (strcmp(argv[i], "-nrepeat") == 0)
    {
      nrepeat = atoi(argv[++i]);
    }
    else if (strcmp(argv[i], "-nteams") == 0)
    {
      nteams = atoi(argv[++i]);
    }
    else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "-help") == 0))
    {
      printf("SAXPY Options:\n");
      printf("  -l <int>:         length of vectors (default: 10000000)\n");
      printf("  -nrepeat <int>:   number of integration invocations (default: 10)\n");
      printf("  -nteams <int>:    number of teams of threads\n");
      printf("  -help (-h):       print this message\n");
    }
  }

  // Initialize Kokkos
  Kokkos::initialize(argc, argv);

  // run test
  test_dotprod(length, nrepeat, nteams);

  // Shutdown Kokkos
  Kokkos::finalize();

  return EXIT_SUCCESS;

} // main
