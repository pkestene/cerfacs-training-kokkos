# Kokkos profiling tools

## build profiling tools

### Use code example from [KokkosTutorial_07_Tools](https://github.com/kokkos/kokkos-tutorials/blob/main/LectureSeries/KokkosTutorial_07_Tools.pdf)

Build and this test code from folder test.
```shell
cd test
mkdir _build; cd _build
module load nvidia/cuda/12.4
module load kokkos/4.4.01-cuda-12.4-gnu-12.3.0-RelWithDebInfo

cmake ..
make
```

use template job.sh to run the example on gpu partition; add `nvprof ./test`
and follow the lecture slides onward.

### SimpleKernelTimer

```shell
git clone git@github.com:kokkos/kokkos-tools.git
cd kokkos-tools/profiling/simple-kernel-timer
make
```

Go back into the `test` folder
```shell
# put the following in the job.sh script
export KOKKOS_TOOLS_LIBS=/path_to_kokkos_tools_sources/profiling/simple-kernel-timer/kp_kernel_timer.so
# run the application
# it will produce *.dat files
```

To read the output *.dat files, jsut use `kp_reader` (built along `kp_kernel_timer.so`)
```shell
kp_reader *.dat
```

For each kernel (identified by name), 5 numbers are displayed:
- total time spent in that kernel
- number of time this kernel was called
- time spent in that kernel per call
- percentage of time spent in that kernel with respect to total spent in all Kokkos kernels
- percentage of time spent in that kernel with respect to total execution time

### Memory tools

Look at slides 19. Try to use tools like `MemoryHighWaterMark`, `MemoryUsage` and `MemoryEvents`

### nvtx-connector

On kraken, e.g.
```shell
module load nvidia/cuda/12.4

git clone git@github.com:kokkos/kokkos-tools.git
cd kokkos-tools/profiling/nvtx-connector
make
```

Let's build a simple CFD application, which code has been _decorated_ with `Kokkos::Profiling::pushRegion` / `Kokkos::Profiling::pop`

```shell
module load nvidia/cuda/12.4
module load kokkos/4.4.01-cuda-12.4-gnu-12.3.0-RelWithDebInfo

git clone git@github.com:pkestene/euler2d_kokkos.git
cd euler2d_kokkos
mkdir -p _build/cuda
cd _build/cuda
make -j 8
cd src
```

Run the application `euler2d` with `job.sh` on GPU partition
```bash
# run application without kokkos profiling
./euler2d ./test_implode.ini

# run the application with kokkos profiling: nvtx-connector
export KOKKOS_TOOLS_LIBS=/full/path/to/kp_nvtx_connector.so
nsys profile --stats=true --trace=nvtx,cuda ./euler2d ./test_implode.ini

# this will create an Nvidia nsys report named report1.nsys-rep
# on kraken, you have to transfer this report on your local machine.
# To visualize the report on the local machine, use:
# nsys-ui report1.nsys-rep
```

## other activities

- see https://github.com/kokkos/kokkos-tutorials/blob/main/LectureSeries/KokkosTutorial_07_Tools.pdf
- build and run [tools_minimd](https://github.com/kokkos/kokkos-tutorials/tree/main/Exercises/tools_minimd) from the [kokkos-tutorials](https://github.com/kokkos/kokkos-tutorials/tree/main) exercises.
