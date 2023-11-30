program saxpy

  use, intrinsic :: iso_c_binding

  use m_compute_kernel
  use m_random_utils ! for init_random_seed

  implicit none

  integer, parameter                        :: length = 10000000
  real(c_double), dimension(:), allocatable :: x,y
  real(c_double)                            :: alpha
  integer, parameter                        :: my_random_seed = 42

  allocate(x(length))
  allocate(y(length))

  call init_random_seed(my_random_seed)
  call init_data(x,y)

  call compute_saxpy(x,y,alpha)

  deallocate(x)
  deallocate(y)

end program saxpy
