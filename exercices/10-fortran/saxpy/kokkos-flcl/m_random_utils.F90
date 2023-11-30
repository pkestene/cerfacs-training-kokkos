module m_random_utils

  implicit none

contains

  !! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  !! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  subroutine init_random_seed(input_seed)

    implicit none

    ! dummy variables
    integer,                intent(in) :: input_seed

    ! local variables
    integer, dimension(:), allocatable :: seeds
    integer                            :: nseeds
    integer                            :: i

    ! get the size of the seed array (for memory allocation)
    call random_seed(size = nseeds)
    allocate(seeds(nseeds))

    seeds = input_seed + 37 * (/ (i - 1, i = 1, nseeds) /)

    ! sets the seeds
    call random_seed(put = seeds)

    ! clear memory
    deallocate(seeds)

  end subroutine init_random_seed

end module m_random_utils
