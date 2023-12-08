// ******************************************
// implicit time stepping implementation of 2D diffusion problem
// Ben Cumming, CSCS
// *****************************************

// A small benchmark app that solves the 2D fisher equation using second-order
// finite differences.

// Syntax: ./main nx ny nt t

#include <algorithm>
#include <fstream>
#include <iostream>

#include <cmath>
#include <cstdlib>
#include <cstring>

#include <omp.h>
#include <stdio.h>

#include "data.h"
#include "linalg.h"
#include "operators.h"
#include "stats.h"

using namespace data;
using namespace linalg;
using namespace operators;
using namespace stats;

// ==============================================================================

// read command line arguments
static void
readcmdline(Discretization & options, int argc, char * argv[])
{
  if (argc < 5 || argc > 6)
  {
    printf("Usage: main nx ny nt t verbose\n");
    printf("  nx        number of gridpoints in x-direction\n");
    printf("  ny        number of gridpoints in y-direction\n");
    printf("  nt        number of timesteps\n");
    printf("  t         total time\n");
    printf("  verbose   (optional) verbose output\n");
    exit(1);
  }

  // read nx
  options.nx = atoi(argv[1]);
  if (options.nx < 1)
  {
    fprintf(stderr, "nx must be positive integer\n");
    exit(-1);
  }

  // read ny
  options.ny = atoi(argv[2]);
  if (options.ny < 1)
  {
    fprintf(stderr, "ny must be positive integer\n");
    exit(-1);
  }

  // read nt
  options.nt = atoi(argv[3]);
  if (options.nt < 1)
  {
    fprintf(stderr, "nt must be positive integer\n");
    exit(-1);
  }

  // read total time
  double t = atof(argv[4]);
  if (t < 0)
  {
    fprintf(stderr, "t must be positive real value\n");
    exit(-1);
  }

  // set verbosity if requested
  verbose_output = false;
  if (argc == 6)
  {
    verbose_output = true;
  }

  // store the parameters
  options.N = options.nx * options.ny;

  // compute timestep size
  options.dt = t / options.nt;

  // compute the distance between grid points
  // assume that x dimension has length 1.0
  options.dx = 1. / (options.nx - 1);

  // set alpha, assume diffusion coefficient D is 1
  options.alpha = (options.dx * options.dx) / (1. * options.dt);
}

// ==============================================================================

int
main(int argc, char * argv[])
{

  Kokkos::initialize(argc, argv);

  // read command line arguments
  readcmdline(options, argc, argv);
  int nx = options.nx;
  int ny = options.ny;
  // int N  = options.N;
  int nt = options.nt;

  // set iteration parameters
  int    max_cg_iters = 200;
  int    max_newton_iters = 50;
  double tolerance = 1.e-6;

  std::cout << "========================================================================"
            << std::endl;
  std::cout << "                      Welcome to mini-stencil!" << std::endl;
  std::cout << "version   :: C++ Kokkos" << std::endl;
  {
    std::ostringstream msg;
    std::cout << "Kokkos configuration" << std::endl;
    if (Kokkos::hwloc::available())
    {
      msg << "hwloc( NUMA[" << Kokkos::hwloc::get_available_numa_count() << "] x CORE["
          << Kokkos::hwloc::get_available_cores_per_numa() << "] x HT["
          << Kokkos::hwloc::get_available_threads_per_core() << "] )" << std::endl;
    }

    Kokkos::print_configuration(msg);

    std::cout << msg.str() << std::endl;
  }
  std::cout << "mesh      :: " << options.nx << " * " << options.ny << " dx = " << options.dx
            << std::endl;
  std::cout << "time      :: " << nt << " time steps from 0 .. " << options.nt * options.dt
            << std::endl;
  ;
  std::cout << "iteration :: "
            << "CG " << max_cg_iters << ", Newton " << max_newton_iters << ", tolerance "
            << tolerance << std::endl;
  ;
  std::cout << "========================================================================"
            << std::endl;

  {
    // Create DataWarehouse object
    DataWarehouse dw(nx, ny);

    Field2d b("b", nx, ny);
    Field2d deltax("deltax", nx, ny);

    // set dirichlet boundary conditions to 0 all around
    ss_fill(dw.bndN, 0.);
    ss_fill(dw.bndS, 0.);
    ss_fill(dw.bndE, 0.);
    ss_fill(dw.bndW, 0.);

    // set the initial condition
    // a circle of concentration 0.1 centred at (xdim/4, ydim/4) with radius
    // no larger than 1/8 of both xdim and ydim
    Field2dHost x_tmp("x_tmp", nx, ny);
    ss_fill(x_tmp, 0.);
    double xc = 1.0 / 4.0;
    double yc = (ny - 1) * options.dx / 4;
    double radius = fmin(xc, yc) / 2.0;
    for (int i = 0; i < nx; i++)
    {
      double x = (i - 1) * options.dx;
      for (int j = 0; j < ny; j++)
      {
        double y = (j - 1) * options.dx;
        if ((x - xc) * (x - xc) + (y - yc) * (y - yc) < radius * radius)
          x_tmp(i, j) = 0.1;
      }
    }
    // "copy" x_tmp into x_new
    // With OpenMP, no real copy happends here
    // With Cuda, x_tmp is uploaded into x_new in GPU global memory
    Kokkos::deep_copy(dw.x_new, x_tmp);

    iters_cg = 0;
    iters_newton = 0;

    // create Conjuguate Gradient context object, and initialize it
    CG_Solver cg_solver(dw);
    cg_solver.init(nx, ny);

    // start timer
    double timespent = -omp_get_wtime();

    // main timeloop
    for (int timestep = 1; timestep <= nt; timestep++)
    {
      // set x_new and x_old to be the solution
      ss_copy(dw.x_old, dw.x_new);

      double residual;
      bool   converged = false;
      int    it;
      for (it = 0; it < max_newton_iters; it++)
      {
        // compute residual : requires both x_new and x_old
        diffusion(dw.x_new, b, dw);
        residual = ss_norm2(b);

        // check for convergence
        if (residual < tolerance)
        {
          converged = true;
          break;
        }

        // solve linear system to get -deltax
        bool cg_converged = false;
        cg_solver.ss_cg(deltax, b, max_cg_iters, tolerance, cg_converged);

        // check that the CG solver converged
        if (!cg_converged)
          break;

        // update solution
        ss_axpy(dw.x_new, -1.0, deltax);
      }
      iters_newton += it + 1;

      // output some statistics
      if (converged && verbose_output)
      {
        std::cout << "step " << timestep << " required " << it << " iterations for residual "
                  << residual << std::endl;
      }
      if (!converged)
      {
        std::cerr << "step " << timestep << " ERROR : nonlinear iterations failed to converge"
                  << std::endl;
        ;
        break;
      }
    }

    // get times
    timespent += omp_get_wtime();

    ////////////////////////////////////////////////////////////////////
    // write final solution to BOV file for visualization
    ////////////////////////////////////////////////////////////////////

    // binary data
    {
      Field2dHost x_new_host("x_new_host", nx, ny);
      Kokkos::deep_copy(x_new_host, dw.x_new);

      FILE * output = fopen("output.bin", "w");

      for (int i = 0; i < nx; ++i)
        for (int j = 0; j < ny; ++j)
        {
          double data = x_new_host(i, j);
          fwrite(&data, sizeof(double), 1, output);
        }
      fclose(output);
    }

    std::ofstream fid("output.bov");
    fid << "TIME: 0.0" << std::endl;
    fid << "DATA_FILE: output.bin" << std::endl;
    fid << "DATA_SIZE: " << options.nx << ", " << options.ny << ", 1" << std::endl;
    ;
    fid << "DATA_FORMAT: DOUBLE" << std::endl;
    fid << "VARIABLE: phi" << std::endl;
    fid << "DATA_ENDIAN: LITTLE" << std::endl;
    fid << "CENTERING: nodal" << std::endl;
    fid << "BRICK_SIZE: 1.0 " << (options.ny - 1) * options.dx << " 1.0" << std::endl;

    // print table sumarizing results
    std::cout << "--------------------------------------------------------------------------------"
              << std::endl;
    std::cout << "simulation took " << timespent << " seconds" << std::endl;
    std::cout << int(iters_cg) << " conjugate gradient iterations, at rate of "
              << float(iters_cg) / timespent << " iters/second" << std::endl;
    std::cout << iters_newton << " newton iterations" << std::endl;
    std::cout << "--------------------------------------------------------------------------------"
              << std::endl;

    std::cout << "Goodbye!" << std::endl;
  }
  Kokkos::finalize();

  return 0;
}
