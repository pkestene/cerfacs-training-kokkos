add_executable(mandelbrot_kokkos_pipeline0
  constants.h
  CudaTimer.h
  HostTimer.h
  kokkos_shared.h
  mandelbrot.h
  main.cpp)

target_link_libraries(mandelbrot_kokkos_pipeline0
  PUBLIC
  Kokkos::kokkos
  )
