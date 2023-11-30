module m_axpy_serial_kernel

  use, intrinsic :: iso_c_binding

  implicit none

contains

  !! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  !! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  subroutine init_data(x,y)

    implicit none

    ! dummy variables
    real(c_double), dimension(:), intent(inout) :: x,y

    ! no local variables

    call random_number(x)
    call random_number(y)

  end subroutine init_data

  !! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  !! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  subroutine compute_saxpy(x,y,alpha)

    implicit none

    ! dummy variables
    real(c_double), dimension(:), intent(in)    :: x
    real(c_double), dimension(:), intent(inout) :: y
    real(c_double),               intent(in)    :: alpha

    ! local variables
    integer(c_int) :: i

    do i=1, size(x)
       y(i) = y(i) + alpha * x(i)
    end do

  end subroutine compute_saxpy

end module m_axpy_serial_kernel
