cmake_minimum_required(VERSION 3.16)

project(
  mandelbrot_kokkos_mdrange
  DESCRIPTION "Mandelbrot set computation (MDRange policy)"
  LANGUAGES CXX C)

#
# default local cmake macro repository
#
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

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

include(compiler_setup)

#
# Make target Kokkos::kokkos available
#
include(build_or_find_kokkos)

#
# Sources
#

# build the miniapp
option (BUILD_SOLUTION "Enable / disable building solution" OFF)

if (BUILD_SOLUTION)
   add_subdirectory(src_solution)
else()
   add_subdirectory(src)
endif()
