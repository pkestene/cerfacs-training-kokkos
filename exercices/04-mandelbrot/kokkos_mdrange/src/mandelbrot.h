#ifndef MANDELBROT_H_
#define MANDELBROT_H_

#include "constants.h"
#include "kokkos_shared.h"

/**
 * Kokkos kernel functor to compute mandelbrot set.
 *
 * This functor is designed to work with a multi-dimensionnal
 * range policy.
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

    /* TODO : mandelbrot computation (import from main.cpp) */
  }

  KOKKOS_INLINE_FUNCTION
  void
  operator()(const int i, const int j) const
  {

    /* TODO : computation body (import from serial/main.cpp) */
  }

  DataArray image;
  float     xmin, ymin, dx, dy, WIDTH, HEIGHT, MAX_COLOR, MAX_ITERS;

}; // end class MandelBrotFunctor

#endif
