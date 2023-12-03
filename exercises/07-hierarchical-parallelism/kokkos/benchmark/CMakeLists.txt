#set(BLA_VENDOR OpenBLAS)
#set(BLA_PREFER_PKGCONFIG ON)
#find_package(BLAS)
find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENBLAS QUIET IMPORTED_TARGET openblas)
if(OPENBLAS_FOUND)
  message("openblas found via pkg-config")
else()
  message(WARNING "openblas NOT FOUND")
  message(STATUS "[FYI] To enable host openblas, make sure to have openblas-serial installed.")
  message(STATUS "[FYI] To make sure openblas-serial will be selected use env variable PKG_CONFIG_PATH")
  message(STATUS "[FYI] e.g. PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/openblas-serial/pkgconfig (on Ubuntu 22.04)")
endif()


add_executable(BatchedDotProduct_bench
  CudaTimer.h
  HipTimer.h
  HostTimer.h
  BatchedDotProduct_bench.cpp)

target_link_libraries(BatchedDotProduct_bench
  PUBLIC
  Kokkos::kokkos
  )

if (OPENBLAS_FOUND)
  target_compile_definitions(BatchedDotProduct_bench PRIVATE -DHOST_CBLAS_ENABLED)
  target_link_libraries(BatchedDotProduct_bench
    PUBLIC
    PkgConfig::OPENBLAS)
endif(OPENBLAS_FOUND)
