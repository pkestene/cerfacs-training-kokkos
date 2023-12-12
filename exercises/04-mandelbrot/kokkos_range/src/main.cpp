#include <cstdio>
#include <iostream>
#include <fstream>

#include <vector>

#include "constants.h"
#include "kokkos_shared.h"
#include "mandelbrot.h"

#ifdef KOKKOS_ENABLE_CUDA
#  include "CudaTimer.h"
using Timer = CudaTimer;
#else // OpenMP
#  include "HostTimer.h"
using Timer = HostTimer;
#endif

#include <unistd.h>

// =========================================================================
// =========================================================================
void
dump_on_screen(unsigned char const * image, Constants const & c)
{
  // print aesthetically, dont read this part
  int xmax = 80;
  int ymax = 60;
  for (int y = 0; y < ymax; y++)
  {
    printf("\n");
    for (int x = 0; x < xmax; x++)
    {
      int index = (y * c.HEIGHT / ymax) * c.WIDTH + (x * c.WIDTH / xmax);
      int val = image[index];

      if (val == 200)
        printf("&");
      else if (val == 42)
        printf("X");
      else if (val > 64)
        printf("#");
      else if (val > 32)
        printf(":");
      else if (val > 8)
        printf(".");
      else
        printf(" ");
    }
  }

  printf("\n");

} // dump_on_screen

// =========================================================================
// =========================================================================
/**
 * Save data in a file using PPM file format.
 */
void
dump_image_ppm(unsigned char const * image, Constants const & c, std::string filename)
{
  FILE * myfile = fopen(filename.c_str(), "w");

  fprintf(myfile, "P6 %d %d 255\n", c.WIDTH, c.HEIGHT);
  for (unsigned int index = 0; index < c.WIDTH * c.HEIGHT; ++index)
  {
    unsigned char data;
    // create an arbitrary RBG code mapping values taken by image
    data = image[index] % 4 * 64;
    fwrite(&data, 1, 1, myfile);
    data = image[index] % 8 * 32;
    fwrite(&data, 1, 1, myfile);
    data = image[index] % 16 * 16;
    fwrite(&data, 1, 1, myfile);
  }

  fclose(myfile);

} // dump_image_ppm

// =========================================================================
// =========================================================================
void
compute_mandelbrot(int & argc, char * argv[])
{
  // parameters
  int  default_size = 1024;
  bool dump_image = false;

  std::vector<std::string> args(argv, argv + argc);

  // Read command line arguments
  for (unsigned int i = 1; i < args.size(); ++i)
  {
    if (args[i] == "-s" or args[i] == "--size")
    {
      default_size = std::stoi(args[i + 1]);
    }
    else if (args[i] == "-d" or args[i] == "--dump")
    {
      dump_image = true;
    }
    else if (args[i] == "-h" or args[i] == "--help")
    {
      printf("Mandelbrot options:\n");
      printf("  --size (-s) <int>:   linear image size (default: 1024)\n");
      printf("  --dump (-d)      :   dump Mandelbrot set to image (default: false)\n");
      printf("  --help (-h)      :   print this message\n");
      exit(EXIT_SUCCESS);
    }
  }

  Constants c = Constants(default_size);

  // Allocate data array for Mandelbrot set computation
  /* TODO 1 : allocate data buffer image and imagehost (host mirror of data)*/
  /* DataArray     image     = ... */
  /* DataArrayHost imageHost = ... */

  /*
   * Actual computation (GPU with CUDA or CPU with OpenMP)
   */
  Timer timer;
  timer.start();

  MandelbrotFunctor functor(image, c);
  Kokkos::parallel_for("compute mandelbrot", c.WIDTH * c.HEIGHT, functor);

  // copy back results from device to host
  /* TODO 2 : deep copy image to imageHost */

  timer.stop();
  printf("Time (%d x %d): %lf seconds.\n", c.WIDTH, c.HEIGHT, timer.elapsed());

  dump_on_screen(imageHost.data(), c);

  // save color ppm file
  if (dump_image)
  {
    dump_image_ppm(imageHost.data(), c, std::string("mandelbrot.ppm"));
  }

} // compute_mandelbrot

// =========================================================================
// =========================================================================
// =========================================================================
int
main(int argc, char * argv[])
{

  /*
   * Initialize kokkos (host + device)
   */
  Kokkos::initialize(argc, argv);

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

    compute_mandelbrot(argc, argv);
  }

  Kokkos::finalize();

  return EXIT_SUCCESS;

} // main
