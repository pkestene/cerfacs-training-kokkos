add_executable(mandelbrot_kokkos_range
  constants.h
  CudaTimer.h
  HostTimer.h
  kokkos_shared.h
  mandelbrot.h
  main.cpp)

target_link_libraries(mandelbrot_kokkos_range
  PUBLIC
  Kokkos::kokkos
  )
