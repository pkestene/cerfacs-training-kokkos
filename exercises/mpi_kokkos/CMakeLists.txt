cmake_minimum_required(VERSION 3.16)
project(
  demo-kokkos-mpi
  DESCRIPTION "MPI + kokkos."
  LANGUAGES CXX C)

#
# default local cmake macro repository
#
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

option(USE_FLOAT "Use float or double (default ON = float)" ON)

#
# set default build type to "Release"
#
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

#
# disallow in-source builds
#
# make sure the user doesn't play dirty with symlinks
get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

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
# always export compile commands database
#
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#
# MPI setup
#
include(config_mpi)

#
# Make target Kokkos::kokkos available
#
include(build_or_find_kokkos)

#
# Build out application
#
add_executable(mpi_kokkos main.cpp)
target_link_libraries(mpi_kokkos PRIVATE Kokkos::kokkos MPI::MPI_CXX)

##################### PRINT CONFIGURE STATUS ######################
message("//===================================================")
message("// ${PROJECT_NAME} build configuration:")
message("//===================================================")
message("")
message("  CMake version          : ${CMAKE_VERSION}")
if (NOT CMAKE_BUILD_TYPE)
  message("  CMake build type       : NOT SET !")
else()
  message("  CMake build type       : ${CMAKE_BUILD_TYPE}")
endif()
message("  CMake install prefix   : ${CMAKE_INSTALL_PREFIX}")
message("  CMake system processor : ${CMAKE_SYSTEM_PROCESSOR}")
message("  CMake system name (OS) : ${CMAKE_SYSTEM_NAME}")
message("")
message("  C++ Compiler : ${CMAKE_CXX_COMPILER_ID} "
                         "${CMAKE_CXX_COMPILER_VERSION} "
                         "${CMAKE_CXX_COMPILER_WRAPPER}")
message("                 ${CMAKE_CXX_COMPILER}")

message("  Kokkos version               = ${Kokkos_VERSION}")
message("  Kokkos_CXX_COMPILER          = ${Kokkos_CXX_COMPILER}")
message("  Kokkos_CXX_COMPILER_ID       = ${Kokkos_CXX_COMPILER_ID}")
message("  Kokkos_CXX_STANDARD          = ${Kokkos_CXX_STANDARD}")
message("  Kokkos_OPTIONS               = ${Kokkos_OPTIONS}")
message("  Kokkos_TPLS                  = ${Kokkos_TPLS}")
message("  Kokkos_DIRS                  = ${Kokkos_DIR}")
