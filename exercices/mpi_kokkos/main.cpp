#include <cstdio>
#include <iostream>
#include <fstream>

#include <mpi.h>
#include "kokkos_shared.h"

#ifdef KOKKOS_ENABLE_CUDA
#  include <cuda.h>
#endif

#include <unistd.h>

// ====================================================================
// ====================================================================
// ====================================================================
int
main(int argc, char * argv[])
{

  // Initialize MPI
  MPI_Init(&argc, &argv);

  /*
   * Initialize kokkos (host + device)
   */
  Kokkos::initialize(argc, argv);

  int rank, nRanks;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

  if (rank == 0)
  {

    std::cout << "##########################\n";
    std::cout << "KOKKOS CONFIG             \n";
    std::cout << "##########################\n";

    std::ostringstream msg;
    std::cout << "Kokkos configuration" << std::endl;
    if (Kokkos::hwloc::available())
    {
      msg << "hwloc( NUMA[" << Kokkos::hwloc::get_available_numa_count() << "] x CORE["
          << Kokkos::hwloc::get_available_cores_per_numa() << "] x HT["
          << Kokkos::hwloc::get_available_threads_per_core() << "] )" << std::endl;
    }
    Kokkos::print_configuration(msg);
    std::cout << msg.str();
    std::cout << "##########################\n";
  }

#ifdef KOKKOS_ENABLE_CUDA
  int cudaDeviceId;
  cudaGetDevice(&cudaDeviceId);

  char host[MPI_MAX_PROCESSOR_NAME];
  int  len;
  MPI_Get_processor_name(host, &len);

  std::cout << "I'm MPI task #" << rank << "running on host " << host << " pinned to GPU #"
            << cudaDeviceId << "\n";
#endif

  Kokkos::finalize();

  MPI_Finalize();

  return EXIT_SUCCESS;

} // main
