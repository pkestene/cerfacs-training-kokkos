add_executable(mandelbrot_kokkos_mdrange
  constants.h
  CudaTimer.h
  HostTimer.h
  kokkos_shared.h
  mandelbrot.h
  main.cpp)

target_link_libraries(mandelbrot_kokkos_mdrange
  PUBLIC
  Kokkos::kokkos
  )
