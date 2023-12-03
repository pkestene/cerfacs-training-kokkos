add_executable(dotprod_team
  dotprod_team.cpp)

target_link_libraries(dotprod_team
  PUBLIC
  Kokkos::kokkos
  )
