//
// test example from Kokkos Lecture 7:
// https://github.com/kokkos/kokkos-tutorials/blob/main/LectureSeries/KokkosTutorial_07_Tools.pdf
// see slide 13 and after
//

#include <cstdlib>

#include <Kokkos_Core.hpp>

void
run_test(int N)
{
  using exec_t = Kokkos::DefaultExecutionSpace;
  using h_exec_t = Kokkos::DefaultHostExecutionSpace;

  Kokkos::View<double *> a("A", N);
  auto                   h_a = Kokkos::create_mirror_view(a);

  Kokkos::Profiling ::pushRegion("Setup");
  Kokkos::parallel_for(
    "Init_A", Kokkos::RangePolicy<h_exec_t>(0, N), KOKKOS_LAMBDA(int i) { h_a(i) = i; });
  deep_copy(a, h_a);
  Kokkos::Profiling ::popRegion();

  Kokkos::Profiling ::pushRegion("Iterate");
  for (int r = 0; r < 10; r++)
  {
    Kokkos::View<double *> tmp("Tmp", N);
    Kokkos::parallel_scan(
      "Kernel_1", Kokkos::RangePolicy<exec_t>(0, N), KOKKOS_LAMBDA(int i, double & lsum, bool f) {
        if (f)
          tmp(i) = lsum;
        lsum += a(i);
      });
    double sum;
    Kokkos::parallel_reduce(
      "Kernel_2",
      Kokkos::RangePolicy<exec_t>(0, N),
      KOKKOS_LAMBDA(int i, double & lsum) { lsum += tmp(i); },
      sum);
  }
  Kokkos::Profiling ::popRegion();

} // run_test

// =========================================================
// =========================================================
// =========================================================
int
main(int argc, char * argv[])
{

  Kokkos::initialize(argc, argv);

  const int N = 1000;
  run_test(N);

  Kokkos::finalize();

  return EXIT_SUCCESS;
}
