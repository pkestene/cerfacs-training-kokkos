The source code is located in [exercises/03-saxpy](https://github.com/pkestene/cerfacs-training-kokkos/tree/main/exercises/03-saxpy)

The code used here is adapted from an old version of the kokkos-tutorial
```shell
git clone https://github.com/kokkos/kokkos-tutorials.git
git checkout 2bbfa49
```
taken from Intro-Full/SNL2015/Exercises/01_AXPY

The code has been slightly modified (to avoid warning and bring modernization).

This handson can be use
- either using the kokkos modulefile, and cmake
- either by building kokkos together with the application; in that case you need to provide env variable `KOKKOS_PATH` to point to the folder where you cloned kokkos sources.
Default value is `KOKKOS_PATH=${HOME}/install/kokkos`

**Activities**
1. Have a look at folder `Serial` and `OpenMP`, build and run application (no cmake, just plain Makefile)
2. Go into folder Kokkos-Lambda, edit file `saxpy.cpp`. There are few TODO, you need to write code to make it run with Kokkos (use cmake for that, cmake is already configured for that)
3. Compare your code with `saxpy_solution.cpp`
4. Run and measure again performance for the OpenMP and Cuda version. Do you see measurable differences ?
4. Have a look at `saxpy_solution2.cpp`. What is the purpose of this other code variant ?
