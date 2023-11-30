cmake_minimum_required(VERSION 3.23)

#
# Allow find_package() to use <PackageName>_ROOT variables,
# either cmake variable or environment variables
# see https://cmake.org/cmake/help/latest/policy/CMP0074.html
#
if(NOT CMAKE_VERSION VERSION_LESS 3.12)
  cmake_policy(SET CMP0074 NEW)
endif()

project(saxpy
  LANGUAGES C CXX Fortran
  DESCRIPTION "SAXPY in Fortran.")

#
# Export compile command to json (for editors like emacs, clion, vim, etc...).
# It allows nice code editing features provided by LSP (Language Server Protocol)
#
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

#
# Prevent build from source directory
#
# make sure the user doesn't play dirty with symlinks
get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

# disallow in-source builds
if("${srcdir}" STREQUAL "${bindir}")
  message("######################################################")
  message("# ${PROJECT_NAME} should not be configured and built in the source directory")
  message("# You must run cmake from a build directory.")
  message("# For example:")
  message("# mkdir _build ; cd _build")
  message("# run cmake from the build directory.")
  message("######################################################")
  message(FATAL_ERROR "Quitting configuration")
endif()

#
# Set default compile optimization flag
#
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(DEFAULT_BUILD_TYPE "Release")
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING
    "Choose the type of build, options are: Debug, Release, RelWithDebInfo and MinSizeRel." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

#
# find dependencies
#
find_package(Kokkos REQUIRED)

# kokkos_check is defined in KokkosConfigCommon.cmake
kokkos_check( DEVICES "OpenMP" RETURN_VALUE KOKKOS_DEVICE_ENABLE_OPENMP)
kokkos_check( DEVICES "Cuda" RETURN_VALUE KOKKOS_DEVICE_ENABLE_CUDA)
message(STATUS "Kokkos::OpenMP enabled ? ${KOKKOS_DEVICE_ENABLE_OPENMP}")
message(STATUS "Kokkos::Cuda   enabled ? ${KOKKOS_DEVICE_ENABLE_CUDA}")

# yakl
find_package(yakl REQUIRED)

# yakl may require MPI and cuda toolkit
find_package(MPI)
if(KOKKOS_DEVICE_ENABLE_CUDA)
  find_package(CUDAToolkit REQUIRED)
endif()

include(cmake/compiler_setup.cmake)

add_executable(saxpy
  m_random_utils.F90
  axpy_kokkos_kernel.cpp
  m_axpy_serial_kernel.F90
  m_axpy_kokkos_kernel.F90
  kokkos-utils.cpp
  m_kokkos_utils.F90
  main.F90)
target_link_libraries(saxpy PRIVATE Kokkos::kokkos yakl::yakl_fortran_interface MPI::MPI_CXX)
