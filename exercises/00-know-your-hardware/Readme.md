# Minimal environment

You can either :
- allocate a interactive session on grace/hopper compute node
- run a batch job on the grace partition

## Allocate a compute node on grace partition for interactive use

```shell
# the following line asks the SLURM manager to allocate a compute node on the grace partition
# for 1 hour, and if enough resources are available, it will open a shell on the reserved
# compute node
salloc --partition=grace -N 1 --time=1:0:0 --gres=gpu:1 -n 24
```

## Run a batch job on grace partition

A example `job.sh` is provided in current directory.

```shell
sbatch ./job.sh
```

## Slurm cheat sheet

See https://slurm.schedmd.com/pdfs/summary.pdf

## Build environment

```shell
# log onto a grace computing node (using salloc, see above)
module load gcc/12.3.0_arm
module load tools/cmake/3.29.3_arm
module load nvidia/cuda/12.4
```

# Know your hardware

The purpose of this simple exercise is just to gather enought information that will allow us to better understand time and performance measurements, and also choose wisely variable `KOKKOS_ARCH` when we will build kokkos library.

We will use a simple `saxpy` algorithm to mesure memory bandwidth.

To run code on the `grace` partition of `calypso`, you can either:
- reserve a compute node, build and run directly on it
- use template script `job.sh` and submit the job on calypso, using command `sbatch job.sh`.

## Know your CPU

You can get information about the CPU using command
- `lscpu`
- `lstopo`

Knowing that the CPU on `grace` partition is an [Nvidia ARM Grace CPU C1](https://resources.nvidia.com/en-us-grace-cpu/data-center-datasheet) with 480 GBytes of RAM, and delivering up to 384 GB/s of memory bandwidth.

**Question:**
- what is the aggregated CPU memory bandwith per socket in gigabytes per second ?

## Know your GPU

Buid and run `deviceQuery.cpp` (directly copied from Nvidia's [CUDA samples](https://github.com/NVIDIA/cuda-samples/blob/master/Samples/1_Utilities/deviceQuery/deviceQuery.cpp):


```shell
# on the grace compute node
module load nvidia/cuda/12.4

nvcc -I. deviceQuery.cpp -o deviceQuery
```

Run `deviceQuery` on the `grace` partition. Cross-check that there are effectively 1 Nvidia GPU of type H100.

**Question:**
- extract information `Memory Bus Width` and `Memory Clock rate` to compute memory bandwith (don't forget to multiply by two to take into account Read and Write memory transfers).
- what is the ratio of memory bandwith between one grace CPU and one Hopper GPU ?


## Explore memory bandwith with saxpy

Use code locate in sub-folder saxpy to evaluate memory bandwith, and see which fraction of the theoretical memory bandwith one can reach.

## Going (much) further on analyzing compute performance

See slides from the [Node-level Performance Engineering](https://moodle.nhr.fau.de/course/view.php?id=55) training at HLRS center.
