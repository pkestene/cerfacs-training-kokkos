#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <omp.h>

// =======================================================================
// =======================================================================
/*
 * Compute dot product between vector b and c
 */
float
dotprod(float * b, float * c, int64_t N)
{
  float sum0 = 0.0;
  float sum1 = 0.0;
#pragma omp target map(to : b[ : N], c[ : N]) map(tofrom : sum0, sum1)
#pragma omp teams  num_teams(2)
  {
    int i;
    if (omp_get_num_teams() != 2)
      abort();
    if (omp_get_team_num() == 0)
    {
#pragma omp parallel for reduction(+ : sum0)
      for (i = 0; i < N / 2; i++)
        sum0 += b[i] * c[i];
    }
    else if (omp_get_team_num() == 1)
    {
#pragma omp        parallel for reduction(+ : sum1)
      for (i = N / 2; i < N; i++)
        sum1 += b[i] * c[i];
    }
  }
  return sum0 + sum1;
} // dotprod

// =======================================================================
// =======================================================================
void
dotprod_test(int64_t length)
{

  auto b = new float[length];
  auto c = new float[length];

  for (int64_t i = 0; i < length; ++i)
  {
    b[i] = 1.0;
    c[i] = 0.5;
  }

  auto res = dotprod(b, c, length);
  printf("Final result is %f\n", res);

  delete[] b;
  delete[] c;

} // dotprod_test

// =======================================================================
// =======================================================================
// =======================================================================
int
main(int argc, char * argv[])
{

  int64_t length = 10000000; // length of vectors

  // Read command line arguments
  for (int i = 0; i < argc; i++)
  {
    if (strcmp(argv[i], "-l") == 0)
    {
      length = std::atol(argv[++i]);
    }
    else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "-help") == 0))
    {
      printf("SAXPY Options:\n");
      printf("  -l <int>:         length of vectors (default: 10000000)\n");
      printf("  -help (-h):       print this message\n");
      exit(EXIT_SUCCESS);
    }
  }

  dotprod_test(length);

  return EXIT_SUCCESS;

} // main
