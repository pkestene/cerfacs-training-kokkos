#ifndef PARAMS_H_
#define PARAMS_H_

#include "common.h"

namespace laplace
{

struct Params
{

  int    NX;
  int    NY;
  int    iter_max;
  real_t tol;

  int ix_start;
  int ix_end;

  int iy_start;
  int iy_end;

  Params()
    : NX(1024)
    , NY(1024)
    , iter_max(1000)
    , tol(1.0e-5)
  {
    ix_start = 1;
    ix_end = NX - 1;
    iy_start = 1;
    iy_end = NY - 1;
  };

  Params(int NX_, int NY_, int iter_max_, real_t tol_)
    : NX(NX_)
    , NY(NY_)
    , iter_max(iter_max_)
    , tol(tol_)
  {
    ix_start = 1;
    ix_end = NX - 1;
    iy_start = 1;
    iy_end = NY - 1;
  };

  Params(int    NX_,
         int    NY_,
         int    iter_max_,
         real_t tol_,
         int    ix_start_,
         int    ix_end_,
         int    iy_start_,
         int    iy_end_)
    : NX(NX_)
    , NY(NY_)
    , iter_max(iter_max_)
    , tol(tol_)
    , ix_start(ix_start_)
    , ix_end(ix_end_)
    , iy_start(iy_start_)
    , iy_end(iy_end_){};

}; // struct Params

} // namespace laplace

#endif // PARAMS_H_
