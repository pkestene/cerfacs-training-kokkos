#ifndef COMMON_H_
#define COMMON_H_

#ifdef USE_DOUBLE
using real_t = double;
#  define fmaxr fmax
#  define fabsr fabs
#  define expr exp
#  define MPI_REAL_TYPE MPI_DOUBLE
#else
using real_t = float;
#  define fmaxr fmaxf
#  define fabsr fabsf
#  define expr expf
#  define MPI_REAL_TYPE MPI_FLOAT
#endif

#endif // COMMON_H_
