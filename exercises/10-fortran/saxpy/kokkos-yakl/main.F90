program saxpy

  use, intrinsic :: iso_c_binding

  use m_kokkos_utils ! for kokkos_initialize, kokkos_finalize

  ! YAKL memory allocator is called "gator"
  use gator_mod

  use m_axpy_serial_kernel
  use m_axpy_kokkos_kernel

  use m_random_utils ! for init_random_seed

  implicit none

  integer, parameter                  :: length = 10000000
  real(c_double), contiguous, pointer :: x(:) => null()
  real(c_double), contiguous, pointer :: y(:) => null()
  real(c_double), contiguous, pointer :: y_serial(:) => null()
  real(c_double)                      :: alpha
  integer                             :: i
  integer, parameter                  :: my_random_seed = 42

  ! initialize kokkos
  write(*,*)'initializing kokkos'
  call kokkos_initialize()

  ! initialize yakl memory allocator
  call gator_init()

  ! allocate fortran array for serial computation and kokkos computation
  write(*,*)'allocating memory'
  call gator_allocate(x,(/length/))
  call gator_allocate(y,(/length/))
  call gator_allocate(y_serial,(/length/))

  alpha = 5

  !
  ! serial computation in pure fortran
  !
  call init_random_seed(my_random_seed)
  call init_data(x,y)
  call compute_saxpy(x,y,alpha)

  ! temporary store serial results (for later cross-check)
  do i = 1, length
     y_serial(i) = y(i)
  end do

  !
  ! parallel computation in kokkos
  !
  call init_random_seed(my_random_seed)
  call init_data(x,y)
  call compute_saxpy_kokkos(c_loc(x), c_loc(y), length, alpha)

  ! check results match
  if ( norm2(y-y_serial) < (1.0e-14)*norm2(y) ) then
    write(*,*)'PASSED y and y_serial are the same after axpys'
  else
    write(*,*)'FAILED y and y_serial are NOT the same after axpys'
    write(*,*)'norm2(y-y_serial)',norm2(y-y_serial)
    write(*,*)'norm2(y)',norm2(y)
  end if

  call gator_deallocate(x)
  call gator_deallocate(y)
  call gator_deallocate(y_serial)

  ! finalize yakl memory allocator
  call gator_finalize()

  ! finalize kokkos
  write(*,*)'finalizing kokkos'
  call kokkos_finalize()

end program saxpy
