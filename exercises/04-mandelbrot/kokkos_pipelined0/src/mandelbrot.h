#ifndef MANDELBROT_H_
#define MANDELBROT_H_

#include "constants.h"
#include "kokkos_shared.h"

/**
 * Kokkos kernel functor to compute mandelbrot set.
 */
class MandelbrotFunctor
{

public:
  MandelbrotFunctor(DataArray image, Constants constants)
    : image(image)
    , xmin(constants.xmin)
    , ymin(constants.ymin)
    , dx(constants.dx)
    , dy(constants.dy)
    , WIDTH(constants.WIDTH)
    , HEIGHT(constants.HEIGHT)
    , MAX_COLOR(constants.MAX_COLOR)
    , MAX_ITERS(constants.MAX_ITERS)
  {}

  KOKKOS_INLINE_FUNCTION
  unsigned char
  mandelbrot(int Px, int Py) const
  {

    float x0 = xmin + Px * dx;
    float y0 = ymin + Py * dy;
    float x = 0.0;
    float y = 0.0;
    int   i;
    for (i = 0; x * x + y * y < 4.0 && i < MAX_ITERS; i++)
    {
      float xtemp = x * x - y * y + x0;
      y = 2 * x * y + y0;
      x = xtemp;
    }
    return (float)MAX_COLOR * i / MAX_ITERS;
  }

  KOKKOS_INLINE_FUNCTION
  void
  operator()(const int & index) const
  {
    int i, j;
    index2coord(index, i, j, WIDTH, HEIGHT);

    image(index) = mandelbrot(i, j);
  }

  DataArray image;
  float     xmin, ymin, dx, dy, WIDTH, HEIGHT, MAX_COLOR, MAX_ITERS;

}; // end class MandelBrotFunctor

#endif
