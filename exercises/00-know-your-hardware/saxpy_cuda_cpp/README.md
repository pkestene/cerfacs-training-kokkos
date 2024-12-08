# Comparing saxpy performance with OpenMP and CUDA implementation

example adapted from:
http://www.speedup.ch/workshops/w38_2009/tutorial.html

Example results on platform
GPU : Quadro K2200
CPU : Intel Xeon CPU E5-1620 v3 @ 3.50GHz
cuda toolkit 10.1

OUR CPU CODE:  4194304 elements,   2.378790 ms per iteration,  3.526 GFLOP/s,  21.159 GB/s
OUR GPU CODE:  4194304 elements,   0.804800 ms per iteration, 10.423 GFLOP/s,  62.539 GB/s
CUBLAS      :  4194304 elements,   0.770336 ms per iteration, 10.890 GFLOP/s,  65.337 GB/s


## Activity

- build environment : `module load gcc/12.3.0_arm tools/cmake/3.29.3_arm nvidia/cuda/12.4`
- build with e.g. : `make CUDA_ARCH=90`
- run environment: you can observe performance when modify the number of OpenMP threads (using `OMP_NUM_THREADS` env variable, from 1 to 72 for the Grace CPU)
- you can also set env var `OMP_PROC_BIND=spread` and `OMP_PLACES=threads`
- run for multiple values of array size, and plot the speed-up of the different parallel implementation versus the serial version.
- when varying the size of the array, can you tell for each architecture (CPU ARM Grace or GPU Nvidia Hopper) how much the run can saturate the memory bandwidth ?
- you can run the batch job: `sbatch ./job.sh`
- Can you comment the CPU performance when varying the input data size ? and the GPU ? Which processor is the fastest (does this question have a unique answer ?) ?
