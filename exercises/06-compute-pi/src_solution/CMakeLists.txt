add_executable(compute_pi
  CudaTimer.h
  HostTimer.h
  compute_pi.cpp)

target_link_libraries(compute_pi
  PUBLIC
  Kokkos::kokkos
  )
