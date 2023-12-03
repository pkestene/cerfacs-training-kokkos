#ifndef PARAMS_H_
#define PARAMS_H_

#include "common.h"

struct Params
{

  int    NX;
  int    NY;
  int    iter_max;
  real_t tol;

  Params()
    : NX(1024)
    , NY(1024)
    , iter_max(1000)
    , tol(1.0e-5)
  {}

  Params(int NX_, int NY_, int iter_max_, real_t tol_)
    : NX(NX_)
    , NY(NY_)
    , iter_max(iter_max_)
    , tol(tol_)
  {}

}; // struct Params

#endif // PARAMS_H_
