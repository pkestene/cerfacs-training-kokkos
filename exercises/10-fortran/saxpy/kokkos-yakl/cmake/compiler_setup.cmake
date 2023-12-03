#
# Minimal compiler setup for warning flags
#

if(CMAKE_Fortran_COMPILER_ID MATCHES "GNU")

  add_compile_options("-Wall;-Wextra")

elseif(CMAKE_Fortran_COMPILER_ID MATCHES "^Intel")

  add_compile_options("-warn all")

elseif(CMAKE_Fortran_COMPILER_ID MATCHES "Clang" OR CMAKE_Fortran_COMPILER_ID MATCHES "IntelLLVM" )

  add_compile_options("-Wall;-Wextra")

elseif(CMAKE_Fortran_COMPILER_ID MATCHES "NVHPC" OR CMAKE_Fortran_COMPILER_ID MATCHES "PGI")

  add_compile_options("-Minform=warn")

else()

  add_compile_options("-Wall;-Wextra")

endif()
