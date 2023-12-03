cmake_minimum_required(VERSION 3.16)
project(
  kokkos-demo-app
  DESCRIPTION "Minimal kokkos application."
  LANGUAGES CXX C)

#
# default local cmake macro repository
#
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

# set default build type to "Release"
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# always export compile commands database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#
# Make target Kokkos::kokkos available
#
include(build_or_find_kokkos)

#
# Build out application
#
add_executable(Kokkos_query_device
  query_device.cpp)
target_link_libraries(Kokkos_query_device Kokkos::kokkos)

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
