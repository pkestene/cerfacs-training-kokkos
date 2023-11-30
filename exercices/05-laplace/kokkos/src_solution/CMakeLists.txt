add_executable(laplace_kokkos
  check_results.h
  common.h
  DataContext.h
  DataContextKokkos.h
  CudaTimer.h
  HostTimer.h
  kokkos_shared.h
  laplace2d_kokkos_kernel.h
  laplace2d_serial_kernel.h
  params.h
  main.cpp)

target_link_libraries(laplace_kokkos
  PUBLIC
  Kokkos::kokkos
  )
