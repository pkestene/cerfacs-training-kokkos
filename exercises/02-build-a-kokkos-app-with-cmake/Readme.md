Here we will review two different ways of using kokkos when building an application:

1. let's integrate kokkos build into the application build
2. let's use an already installed

# Use kokkos as a regular library with a modulefile

In [exercise 01](../01-build-kokkos) we have just built and installed 3 versions of Kokkos using different compile toolchains (GNU, Intel/LLVM, nvcc), different low-level hardware backends (OpenMP, CUDA).

These installed versions of kokkos can re-used through a modulefile.
Let's checked that and review how we can setup the cmake build system of an application to handle that.

## modulefiles setup

Let's first make sure the modulefiles are functionnal. To do that:

Assuming you cloned `cerfacs-training-kokkos` project in your $HOME directory

```shell
module use $HOME/cerfacs-training-kokkos/modulefiles
module avail kokkos
```

You should see that three new modulefiles are available:
```text
 > $ module av kokkos

---------- /home/exter/kestener/cerfacs-training-kokkos/modulefiles --------
kokkos/4.4.01-cuda-12.4-gnu-12.3.0-RelWithDebInfo
kokkos/4.4.01-openmp-gnu-12.3.0-RelWithDebInfo
```

# Integrate kokkos into a cmake-based application

Go into `kokkos-demo-application`, we will build a kokkos demo application (this is just again `query_device` application).

Let's assume we want to use both Kokkos OpenMP and CUDA backends.

Let's first review the cmake macro that allow to chose if we want to build kokkos as part of the application or use an already installed version of kokkos

```shell
cd kokkos-demo-application
# open file `cmake/build_or_find_kokkos.cmake`
# we will comment this macro during hands-on session
```

## Build application and Kokkos

The most simple way of using kokkos, is to let the application build kokkos first, and then the application.

```shell
# environement setup
module load nvidia/cuda/12.4

cd kokkos-demo-application
mkdir -p _build/app_and_kokkos
cd _build/app_and_kokkos
cmake -DDEMO_APP_KOKKOS_BUILD:BOOL=ON -DDEMO_APP_KOKKOS_BACKEND=Cuda ../..
make -j 8
# run app directly on grace node
./Kokkos_query_device
# run app from login node (edit job.sh)
cp ../../../../job.sh .
sbatch job.sh
```

## Build application and used kokkos (already installed)

```shell
# environement setup
module load nvidia/cuda/12.4
module load kokkos/4.4.01-cuda-12.4-gnu-12.3.0-RelWithDebInfo

cd kokkos-demo-application
mkdir -p _build/app
cd _build/app
cmake ../..
make -j 8
# run app directly on grace node
./Kokkos_query_device
# run app from login node (edit job.sh)
cp ../../../../job.sh .
sbatch job.sh
```

Watch carefully the output and make sure you have the same results.

- just to practive, try to set environment variable to e.g. `CUDA_VISIBLES_DEVICES=0`, re-run again the application, what do you see ?
- just to practice, try to change the number of OpenMP threads, have a look at the output

## Other activities

- just to practice, after making sure the kokkos modulefile is not loaded, try to rebuild the application by activating e.g. only the Kokkos::Serial device.
- add a printf in {\tt main} to display the name of {\tt DefaultExecutionSpace} and {\tt DefaultHostExecutionSpace} (just for cross checking)
```c++
printf("Default execution space is %s\n",Kokkos::DefaultExecutionSpace::name());
printf("Default memory    space is %s\n",Kokkos::DefaultExecutionSpace::memory_space::name());
```
