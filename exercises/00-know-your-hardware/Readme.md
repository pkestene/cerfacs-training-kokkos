# Minimal environment

```shell
# put this at the end of your .bash_profile on kraken
# we want to use a fairly recent version of the GNU compiler
module load cmake/3.25.0
```

# Know your hardware

The purpose of this simple exercise is just to gather enought information that will allow us to better understand time and performance measurements, and also choose wisely variable `KOKKOS_ARCH` when we will build kokkos library.

We will use a simple `saxpy` algorithm and mesure memory bandwidth.

## Know your CPU

You can get information about the CPU using command
- `lscpu`
- `lstopo`

Knowing that the CPU on `hpcai` partition is an [AMD EPYC 7502P 32-Core Processor](https://www.amd.com/fr/products/cpu/amd-epyc-7502p) with 8 populated memory channel operating at 3200 MT/s.

**Question:**
- what is the aggregated CPU memory bandwith per socket in gigabytes per second ?

## Know your GPU

Buid and run `deviceQuery.cpp` (directly copied from Nvidia's [CUDA samples](https://github.com/NVIDIA/cuda-samples/blob/master/Samples/1_Utilities/deviceQuery/deviceQuery.cpp):


```shell
module load cuda/11.8

nvcc -I. deviceQuery.cpp -o deviceQuery
```

Run `deviceQuery` on `hpcai`. Cross-check that there are effectively 2 Nvidia GPU of type A100.

**Question:**
- extract information `Memory Bus Width` and `Memory Clock rate` to compute memory bandwith (don't forget to multiply by to take into account Read and Write memory transfers).
- what is the ratio of memory bandwith between one CPU socket and one A100 GPU ?


## Explore memory bandwith with saxpy

Use code locate in sub-folder saxpy to evaluate memory bandwith, and see which fraction of the theoretical memory bandwith one can reach.

## Going (much) further on analyzing compute performance

See slides from the [Node-level Performance Engineering](https://moodle.nhr.fau.de/course/view.php?id=55) training at HLRS center.
