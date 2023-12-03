program saxpy

  use, intrinsic :: iso_c_binding

  use flcl_mod
  use flcl_util_kokkos_mod

  use m_axpy_serial_kernel
  use m_axpy_kokkos_kernel

  use m_random_utils ! for init_random_seed

  implicit none

  integer, parameter                        :: length = 10000000
  real(c_double), dimension(:), allocatable :: x
  real(c_double), dimension(:), allocatable :: y
  real(c_double)                            :: alpha

  !! remeber that x_view and x_ptr atually point to the same memory area

  !! kokkos view (actually UVM is Kokkos::Cuda activated)
  type(view_r64_1d_t)                       :: x_view
  type(view_r64_1d_t)                       :: y_view

  !! pointers for C/Fortran interoperability
  real(c_double), pointer, dimension(:)     :: x_ptr => null()
  real(c_double), pointer, dimension(:)     :: y_ptr => null()

  integer, parameter                        :: my_random_seed = 42

  ! initialize kokkos
  write(*,*)'initializing kokkos'
  call kokkos_initialize()

  ! allocate fortran array for serial computation
  allocate(x(length))
  allocate(y(length))

  ! allocate kokkos views
  write(*,*)'allocating kokkos views and initialize pointers'
  call kokkos_allocate_view( y_ptr, y_view, 'y_ptr', int(length, c_size_t) )
  call kokkos_allocate_view( x_ptr, x_view, 'x_ptr', int(length, c_size_t) )

  alpha = 5

  !
  ! serial computation in pure fortran
  !
  call init_random_seed(my_random_seed)
  call init_data(x,y)
  call compute_saxpy(x,y,alpha)

  !
  ! parallel computation in kokkos
  !
  call init_random_seed(my_random_seed)
  call init_data(x_ptr,y_ptr)
  call compute_saxpy_kokkos(x_view, y_view, alpha)

  ! check results match
  if ( norm2(y-y_ptr) < (1.0e-14)*norm2(y) ) then
    write(*,*)'PASSED y (host) and y_ptr (device) are the same after axpys'
  else
    write(*,*)'FAILED y (host) and y_ptr (device) are NOT the same after axpys'
    write(*,*)'norm2(y-y_ptr)',norm2(y-y_ptr)
    write(*,*)'norm2(y)',norm2(y)
  end if

  deallocate(x)
  deallocate(y)

  ! finalize kokkos
  write(*,*)'finalizing kokkos'
  call kokkos_finalize()

end program saxpy
