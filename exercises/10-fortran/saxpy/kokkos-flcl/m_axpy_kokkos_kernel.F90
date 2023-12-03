module m_axpy_kokkos_kernel

  use, intrinsic :: iso_c_binding

  use :: flcl_mod

  implicit none

  public

  interface
     subroutine axpy_kernel( x_ptr, y_ptr, alpha ) &
          & bind(c, name='axpy_kokkos_kernel')
       use, intrinsic :: iso_c_binding
       use :: flcl_mod
       type(c_ptr),    intent(in) :: x_ptr
       type(c_ptr),    intent(in) :: y_ptr
       real(c_double), intent(in) :: alpha
     end subroutine axpy_kernel
  end interface

contains

  subroutine compute_saxpy_kokkos( x_view, y_view, alpha )
    use, intrinsic :: iso_c_binding
    use :: flcl_mod
    implicit none
    type(view_r64_1d_t), intent(in)    :: x_view
    type(view_r64_1d_t), intent(inout) :: y_view
    real(c_double),      intent(in)    :: alpha

    call axpy_kernel(x_view%ptr(), y_view%ptr(), alpha)

  end subroutine compute_saxpy_kokkos

end module m_axpy_kokkos_kernel
