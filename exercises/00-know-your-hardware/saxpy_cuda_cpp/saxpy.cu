/**
 * Compute saxpy
 * - on CPU : serial and OpenMP version
 * - on GPU : first using CUDA, then library CuBLAS
 *
 * compare timings.
 *
 */

// =========================
// standard imports
// =========================
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iomanip>
#include <type_traits> // for std::is_same_v

// =========================
// CUDA imports
// =========================
#include <cuda_runtime.h>
#include <cublas_v2.h>

// =========================
// OpenMP imports
// =========================
#ifdef _OPENMP
#  include <omp.h>
#endif

// =========================
// our imports
// =========================
#include "my_cuda_utils.h"
#include "HostTimer.h"
#include "CudaTimer.h"

// =========================
// global variables and configuration section
// =========================

// number of repetitions of the timing loop
// (CPU timers only have a ~ms resolution)
static int numTimingReps = 100;

#ifdef USE_FLOAT
using real_t = float;
#else
using real_t = double;
#endif

/**
 * @brief Convert a size into a human readable string
 *
 * see
 * https://github.com/NVIDIA/cccl/blob/main/cudax/include/cuda/experimental/__stf/utility/pretty_print.cuh
 */
inline ::std::string
pretty_print_prefix(size_t num_elem)
{
  const char * units[] = { " ", "K", "M", "G", "T" };
  size_t       size = sizeof(units) / sizeof(char *);
  int          i = 0;

  double pretty_size = static_cast<double>(num_elem);
  while (pretty_size >= 1024.0 && static_cast<size_t>(i) < size - 1)
  {
    pretty_size /= 1024.0;
    ++i;
  }

  ::std::ostringstream out;
  out << ::std::fixed << ::std::setprecision(2) << pretty_size << ' ' << units[i];
  return out.str();
}

// =========================
// kernel function (CPU) - serial
// =========================
void
saxpy_serial(int n, real_t alpha, const real_t * x, real_t * y)
{

  for (size_t i = 0; i < n; ++i)
    y[i] = alpha * x[i] + y[i];
}

// =========================
// kernel function (CPU) - OpenMP
// =========================
void
saxpy_openmp(int n, real_t alpha, const real_t * x, real_t * y)
{

#pragma omp parallel for
#pragma ivdep
  for (size_t i = 0; i < n; ++i)
    y[i] = alpha * x[i] + y[i];
}


// =========================
// kernel function (CUDA device)
// =========================
__global__ void
saxpy_cuda(int n, real_t alpha, const real_t * x, real_t * y)
{
  // compute the global index in the vector from
  // the number of the current block, blockIdx,
  // the number of threads per block, blockDim,
  // and the number of the current thread within the block, threadIdx
  size_t i = blockIdx.x * blockDim.x + threadIdx.x;

  // except for special cases, the total number of threads in all blocks
  // adds up to more than the vector length n, so this conditional is
  // EXTREMELY important to avoid writing past the allocated memory for
  // the vector y.
  if (i < n)
    y[i] = alpha * x[i] + y[i];
}


// =========================
// main routine
// =========================
int
main(int argc, char ** argv)
{

  // base-2 log of array size
  int log2N = argc > 1 ? atoi(argv[1]) : 22;

  // problem size (vector length) N
  // size_t N = 1234567;
  size_t N = 1 << log2N;
  // size_t N = 40000;

  HostTimer cpuTimer;
  HostTimer ompTimer;
  CudaTimer gpuTimer;

  if constexpr (std::is_same_v<real_t, float>)
    printf("Using data type: float\n");
  else
    printf("Using data type: double\n");

#ifdef _OPENMP
  int         threadId;
#  pragma omp parallel private(threadId)
  {
    threadId = omp_get_thread_num();
    if (threadId == 0)
      printf("Using %d OpenMP threads\n", omp_get_num_threads());
  }
#else
  printf("OpenMP not activated\n");
#endif

  // =========================
  // (1) initialisations:
  //     implemented in my_cuda_utils.c
  // =========================
  initCuda(0);


  // =========================
  // (2) allocate memory on host (main CPU memory) and device,
  //     h_ denotes data residing on the host, d_ on device
  // =========================
  real_t * h_x = (real_t *)malloc(N * sizeof(real_t));
  real_t * h_y = (real_t *)malloc(N * sizeof(real_t));
  real_t * d_x;
  cudaMalloc((void **)&d_x, N * sizeof(real_t));
  real_t * d_y;
  cudaMalloc((void **)&d_y, N * sizeof(real_t));
  checkErrors("memory allocation");


  // =========================
  // (3) initialise data on the CPU
  // =========================
#pragma omp parallel for
  for (size_t i = 0; i < N; ++i)
  {
    h_x[i] = 1.0f + i;
    h_y[i] = (real_t)(N - i + 1);
  }


  // =========================
  // (4) copy data to device
  // =========================
  cudaMemcpy(d_x, h_x, N * sizeof(real_t), cudaMemcpyHostToDevice);
  cudaMemcpy(d_y, h_y, N * sizeof(real_t), cudaMemcpyHostToDevice);
  checkErrors("copy data to device");

  real_t alpha = 2.0;

  // =========================
  // (5a) perform computation on host - SERIAL
  //     use our straight forward code
  //     and our utility functions to time everything,
  //     note that gettimeofday has ~ms resolution, so
  //     perform everything in a loop to minimise
  //     timing noise
  // =========================
  cpuTimer.start();
  for (int iter = 0; iter < numTimingReps; ++iter)
    saxpy_serial(N, alpha, h_x, h_y);
  cpuTimer.stop();
  double elapsed = cpuTimer.elapsed();
  printf("CPU CODE (Serial): 2^%d = %8ld (= %s) elements, %10.6f ms per iteration, %6.3f GFLOP/s, "
         "%7.3f GB/s\n",
         log2N,
         N,
         pretty_print_prefix(N).c_str(),
         (elapsed * 1000.0) / (double)numTimingReps,
         2.0 * N * numTimingReps / (elapsed * 1e9),
         3.0 * N * sizeof(real_t) * numTimingReps / (elapsed * 1e9));

  // =========================
  // (5b) perform computation on host - OpenMP
  // =========================
  ompTimer.start();
  for (int iter = 0; iter < numTimingReps; ++iter)
    saxpy_openmp(N, alpha, h_x, h_y);
  ompTimer.stop();
  elapsed = ompTimer.elapsed();
  printf("CPU CODE (OpenMP): 2^%d = %8ld (= %s) elements, %10.6f ms per iteration, %6.3f GFLOP/s, "
         "%7.3f GB/s\n",
         log2N,
         N,
         pretty_print_prefix(N).c_str(),
         (elapsed * 1000.0) / (double)numTimingReps,
         2.0 * N * numTimingReps / (elapsed * 1e9),
         3.0 * N * sizeof(real_t) * numTimingReps / (elapsed * 1e9));


  // =========================
  // (7) perform computation on device, our implementation
  //     use CUDA events to time the execution:
  //     (a) insert "tag" into instruction stream
  //     (b) execute kernel
  //     (c) insert another tag into instruction stream
  //     (d) synchronize (ie, wait for) this tag (event)
  //     CUDA events have a resolution of ~0.5us
  // =========================
  real_t time;

  // Mapping onto the device:
  // - each thread computes one element of the output array in situ
  // - all threads and blocks are independent
  // - use 256 threads per block
  // - use as many blocks as necessary (the last block is not entirely
  //   full if n is not a multiple of 256)
  int numThreadsPerBlock = 128;
  int numBlocks = (N + numThreadsPerBlock - 1) / numThreadsPerBlock;

  gpuTimer.start();
  saxpy_cuda<<<numBlocks, numThreadsPerBlock>>>(N, alpha, d_x, d_y);
  gpuTimer.stop();
  time = gpuTimer.elapsed();
  printf("GPU CODE (CUDA)  : 2^%d = %8ld (= %s) elements, %10.6f ms per iteration, %6.3f GFLOP/s, "
         "%7.3f GB/s\n",
         log2N,
         N,
         pretty_print_prefix(N).c_str(),
         time * 1000,
         2.0 * N / (time * 1e9),
         3.0 * N * sizeof(real_t) / (time * 1e9));


  // =========================
  // (8) read back result from device into temp vector
  // =========================
  real_t * h_z = (real_t *)malloc(N * sizeof(real_t));
  cudaMemcpy(h_z, d_y, N * sizeof(real_t), cudaMemcpyDeviceToHost);
  checkErrors("copy data from device");


  // =========================
  // (9) perform computation on device, CUBLAS
  // =========================
  {
    [[maybe_unused]] cublasStatus_t status;
    cublasHandle_t                  handle;
    status = cublasCreate(&handle);

    gpuTimer.reset();
    if constexpr (std::is_same_v<real_t, float>)
    {
      // printf("%d ", std::is_same<float, real_t>::value);
      gpuTimer.start();
      cublasSaxpy(handle, N, (float *)&alpha, (float *)d_x, 1, (float *)d_y, 1);
      gpuTimer.stop();
    }
    else if constexpr (std::is_same_v<real_t, double>)
    {
      gpuTimer.start();
      cublasDaxpy(handle, N, (double *)&alpha, (double *)d_x, 1, (double *)d_y, 1);
      gpuTimer.stop();
    }
    time = gpuTimer.elapsed();
    printf(
      "GPU CODE (CUBLAS): 2^%d = %8ld (= %s) elements, %10.6f ms per iteration, %6.3f GFLOP/s, "
      "%7.3f "
      "GB/s\n",
      log2N,
      N,
      pretty_print_prefix(N).c_str(),
      time * 1000,
      2.0 * N / (time * 1e9),
      3.0 * N * sizeof(real_t) / (time * 1e9));
    status = cublasDestroy(handle);
  }

  // =========================
  // (10) perform result comparison
  //      we need to re-run the CPU code because
  //      it has been executed 1000 times before
  // =========================
  int errorCount = 0;
  for (size_t i = 0; i < N; ++i)
  {
    h_x[i] = 1.0f + i;
    h_y[i] = (real_t)(N - i + 1);
  }
  saxpy_serial(N, alpha, h_x, h_y);
  for (size_t i = 0; i < N; ++i)
  {
    if (abs(h_y[i] - h_z[i]) > 1e-6)
      errorCount = errorCount + 1;
  }
  if (errorCount > 0)
    printf("Result comparison failed.\n");
  else
    printf("Result comparison passed.\n");


  // =========================
  // (11) clean up, free memory
  // =========================
  free(h_x);
  free(h_y);
  free(h_z);
  cudaFree(d_x);
  cudaFree(d_y);

  return 0;
}
