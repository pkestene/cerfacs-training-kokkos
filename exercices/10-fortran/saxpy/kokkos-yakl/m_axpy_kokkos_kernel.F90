module m_axpy_kokkos_kernel

  use, intrinsic :: iso_c_binding

  implicit none

  public

  interface
     subroutine compute_saxpy_kokkos( x_ptr, y_ptr, length, alpha ) &
          & bind(c, name='axpy_kokkos_kernel')
       use, intrinsic :: iso_c_binding
       type(c_ptr),        value, intent(in) :: x_ptr
       type(c_ptr),        value, intent(in) :: y_ptr
       integer(c_int32_t), value, intent(in) :: length
       real(c_double),     value, intent(in) :: alpha
     end subroutine compute_saxpy_kokkos
  end interface

end module m_axpy_kokkos_kernel
