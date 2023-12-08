// linear algebra subroutines
// Ben Cumming @ CSCS

#include <iostream>

#include <cmath>
#include <cstdio>

#include "linalg.h"
#include "operators.h"
#include "stats.h"
#include "data.h"

namespace linalg
{

CG_Solver::CG_Solver(DataWarehouse & dw_)
  : cg_initialized(false)
  , dw(dw_)
  , r()
  , Ap()
  , p()
  , Fx()
  , Fxold()
  , v()
  , xold()
{}

CG_Solver::~CG_Solver() {}

// initialize temporary storage fields used by the cg solver
// I do this here so that the fields are persistent between calls
// to the CG solver. This is useful if we want to avoid malloc/free calls
// on the device for the OpenACC implementation (feel free to suggest a better
// method for doing this)
void
CG_Solver::init(int nx, int ny)
{

  // allocate memory for Ap, r, p, Fx, Fxold, v, xold
  /* TODO */

  cg_initialized = true;

} // CG_Solver::init

////////////////////////////////////////////////////////////////////////////////
//  blas level 1 reductions
////////////////////////////////////////////////////////////////////////////////

// computes the inner product of x and y
// x and y are vectors on length N
double
ss_dot(Field2d x, Field2d y)
{
  const int Nx = x.extent(0);
  const int Ny = x.extent(1);

  double result = 0;

  /* TODO */
  /*
    for (int i = 0; i < N; i++)
        result += x[i] * x[i];
  */

  return result;
}

// computes the 2-norm of x
// x is a vector on length N
double
ss_norm2(Field2d x)
{

  const int Nx = x.extent(0);
  const int Ny = x.extent(1);

  double result = 0;

  /* TODO */
  /*
  for (int i = 0; i < N; i++)
        result += x[i] * x[i];
  */

  return sqrt(result);
}

// sets entries in a vector to value
// x is a vector on length N
// value is th
void
ss_fill(Field1d x, double value)
{

  /* TODO */
  /*
  for (int i = 0; i < N; i++)
        x[i] = value;
  */
}

void
ss_fill(Field2dHost x, double value)
{

  const int Nx = x.extent(0);
  const int Ny = x.extent(1);

  for (int i = 0; i < Nx; ++i)
    for (int j = 0; j < Ny; ++j)
      x(i, j) = value;
}

void
ss_fill(Field2d x, double value)
{

  const int Nx = x.extent(0);
  const int Ny = x.extent(1);

  /* TODO */
}

////////////////////////////////////////////////////////////////////////////////
//  blas level 1 vector-vector operations
////////////////////////////////////////////////////////////////////////////////

// computes y := alpha*x + y
// x and y are vectors on length N
// alpha is a scalar
void
ss_axpy(Field2d y, double alpha, Field2d x)
{

  const int Nx = x.extent(0);
  const int Ny = x.extent(1);

  /* TODO */
  /*
    for (int i = 0; i < N; i++)
        y[i] += alpha * x[i];
  */
}

// computes y = x + alpha*(l-r)
// y, x, l and r are vectors of length N
// alpha is a scalar
void
ss_add_scaled_diff(Field2d y, Field2d x, double alpha, Field2d l, Field2d r)
{
  const int Nx = x.extent(0);
  const int Ny = x.extent(1);

  /* TODO */
  /*
  for (int i = 0; i < N; i++)
        y[i] = x[i] + alpha * (l[i] - r[i]);
  */
}

// computes y = alpha*(l-r)
// y, l and r are vectors of length N
// alpha is a scalar
void
ss_scaled_diff(Field2d y, double alpha, Field2d l, Field2d r)
{

  const int Nx = y.extent(0);
  const int Ny = y.extent(1);

  /* TODO */
  /*
  for (int i = 0; i < N; i++)
    y[i] = alpha * (l[i] - r[i]);
  */
}

// computes y := alpha*x
// alpha is scalar
// y and x are vectors on length n
void
ss_scale(Field2d y, double alpha, Field2d x)
{
  const int Nx = x.extent(0);
  const int Ny = x.extent(1);

  /* TODO */
  /*
    for (int i = 0; i < N; i++)
        y[i] = alpha * x[i];
  */
}

// computes linear combination of two vectors y := alpha*x + beta*z
// alpha and beta are scalar
// y, x and z are vectors on length n
void
ss_lcomb(Field2d y, double alpha, Field2d x, double beta, Field2d z)
{
  const int Nx = x.extent(0);
  const int Ny = x.extent(1);

  /* TODO */
  /*
  for (int i = 0; i < N; i++)
    y[i] = alpha * x[i] + beta * z[i];
  */
}

// copy one vector into another y := x
// x and y are vectors of length N
void
ss_copy(Field2d y, Field2d x)
{

  /* TODO : use kokkos deep_copy API*/
}

// conjugate gradient solver
// solve the linear system A*x = b for x
// the matrix A is implicit in the objective function for the diffusion equation
// the value in x constitute the "first guess" at the solution
// x(N)
// ON ENTRY contains the initial guess for the solution
// ON EXIT  contains the solution
void
CG_Solver::ss_cg(Field2d x, Field2d b, int maxiters, double tol, bool & success)
{
  using namespace operators;

  // this is the dimension of the linear system that we are to solve
  using data::options;
  // int N = options.N;
  int nx = options.nx;
  int ny = options.ny;

  if (!cg_initialized)
    init(nx, ny);

  // epslion value use for matrix-vector approximation
  double eps = 1.e-8;
  double eps_inv = 1. / eps;

  // allocate memory for temporary storage
  ss_fill(Fx, 0.0);
  ss_fill(Fxold, 0.0);
  ss_copy(xold, x);

  // matrix vector multiplication is approximated with
  // A*v = 1/epsilon * ( F(x+epsilon*v) - F(x) )
  //     = 1/epsilon * ( F(x+epsilon*v) - Fxold )
  // we compute Fxold at startup
  // we have to keep x so that we can compute the F(x+exps*v)
  diffusion(x, Fxold, dw);

  // v = x + epsilon*x
  ss_scale(v, 1.0 + eps, x);

  // Fx = F(v)
  diffusion(v, Fx, dw);

  // r = b - A*x
  // where A*x = (Fx-Fxold)/eps
  ss_add_scaled_diff(r, b, -eps_inv, Fx, Fxold);

  // p = r
  ss_copy(p, r);

  // rold = <r,r>
  double rold = ss_dot(r, r), rnew = rold;

  // check for convergence
  success = false;
  if (sqrt(rold) < tol)
  {
    success = true;
    return;
  }

  int iter;
  for (iter = 0; iter < maxiters; iter++)
  {
    // Ap = A*p
    ss_lcomb(v, 1.0, xold, eps, p);
    diffusion(v, Fx, dw);
    ss_scaled_diff(Ap, eps_inv, Fx, Fxold);

    // alpha = rold / p'*Ap
    double alpha = rold / ss_dot(p, Ap);

    // x += alpha*p
    ss_axpy(x, alpha, p);

    // r -= alpha*Ap
    ss_axpy(r, -alpha, Ap);

    // find new norm
    rnew = ss_dot(r, r);

    // test for convergence
    if (sqrt(rnew) < tol)
    {
      success = true;
      break;
    }

    // p = r + rnew.rold * p
    ss_lcomb(p, 1.0, r, rnew / rold, p);

    rold = rnew;
  }
  stats::iters_cg += iter + 1;

  if (!success)
    std::cerr << "ERROR: CG failed to converge" << std::endl;

} // CG_Solver::ss_cg

} // namespace linalg
