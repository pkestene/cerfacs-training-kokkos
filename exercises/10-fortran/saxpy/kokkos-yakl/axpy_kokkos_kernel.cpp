#include <Kokkos_Core.hpp>

using memory_space = Kokkos::DefaultExecutionSpace::memory_space;
using memory_trait = Kokkos::MemoryTraits<Kokkos::Unmanaged>;

// generic template aliases for multidimensional views
template <class value_t>
using View_1d = Kokkos::View<value_t *, memory_space, memory_trait>;

// generic template aliases for multidimensional const views
template <class value_t>
using View_1d_const = Kokkos::View<const value_t *, memory_space, memory_trait>;


extern "C"
{

  void
  axpy_kokkos_kernel(double * x, double * y, int length, double alpha)
  {
    // just cross-checking
    printf("Kokkos default execution space is %s\n", Kokkos::DefaultExecutionSpace::name());

    // create views
    auto x_view = View_1d_const<double>(x, length);
    auto y_view = View_1d<double>(y, length);

    Kokkos::parallel_for(
      "axpy", Kokkos::RangePolicy<>(0, y_view.extent(0)), KOKKOS_LAMBDA(const size_t idx) {
        y_view(idx) += alpha * x_view(idx);
      });

    // make sure data can be reused on host
    Kokkos::fence();

    return;
  }
}
