#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>

#include <vector>

#include "constants.h"

#include "HostTimer.h"

#include <unistd.h>

// =========================================================================
// =========================================================================
/*
 * compute Mandelbrot pixelwise.
 * some global variables are used as parameters
 */
unsigned char
mandelbrot(int Px, int Py, Constants const & c)
{

  float        x0 = c.xmin + Px * c.dx;
  float        y0 = c.ymin + Py * c.dy;
  float        x = 0.0;
  float        y = 0.0;
  unsigned int i = 0;

  for (i = 0; x * x + y * y < 4.0 and i < c.MAX_ITERS; ++i)
  {
    float xtemp = x * x - y * y + x0;
    y = 2 * x * y + y0;
    x = xtemp;
  }
  return (float)c.MAX_COLOR * i / c.MAX_ITERS;
} // mandelbrot

// =========================================================================
// =========================================================================
/* convert a linear index to cartesian coordinate */
inline void
index2coord(int index, int & i, int & j, [[maybe_unused]] int Nx, int Ny)
{
  i = index / Ny;
  j = index - i * Ny;
} // index2coord

// =========================================================================
// =========================================================================
void
dump_on_screen(unsigned char const * image, Constants const c)
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
// =========================================================================
int
main(int argc, char * argv[])
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
  std::vector<unsigned char> image(c.WIDTH * c.HEIGHT);

  /*
   * Actual computation
   */
  HostTimer timer;
  timer.start();

  for (unsigned int index = 0; index < c.WIDTH * c.HEIGHT; ++index)
  {

    int i, j;
    index2coord(index, i, j, c.WIDTH, c.HEIGHT);

    image[index] = mandelbrot(i, j, c);
  }

  printf("end of loop reached ...\n");

  timer.stop();
  printf("Time: %lf seconds.\n", timer.elapsed());

  dump_on_screen(image.data(), c);

  // save color ppm file
  if (dump_image)
  {
    dump_image_ppm(image.data(), c, std::string("mandelbrot.ppm"));
  }

  return EXIT_SUCCESS;

} // main
