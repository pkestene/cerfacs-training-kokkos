# Minimal environment

```shell
# put this at the end of your .bash_profile on kraken
# we want to use a fairly recent version of the GNU compiler
module unload compiler
module load compiler/gcc/11.2.0

module unload mpi

module unload tools/cmake
module load tools/cmake/3.23.0

module unload tools/git
module load tools/git/2.37.1
```

# Know your hardware

The purpose of this simple exercise is just to gather enought information that will allow us to better understand time and performance measurements, and also choose wisely variable `KOKKOS_ARCH` when we will build kokkos library.

We will use a simple `saxpy` algorithm and mesure memory bandwidth.

To run code on the `gpua30` partition of `kraken`, use template script `job.sh` and submit the job on kraken, using command `sbatch job.sh`.

## Know your CPU

You can get information about the CPU using command
- `lscpu`
- `lstopo`

Knowing that the CPU on `gpua30` partition is an [Intel Xeon Gold 6326 CPU](https://www.intel.fr/content/www/fr/fr/products/sku/215274/intel-xeon-gold-6326-processor-24m-cache-2-90-ghz/specifications.html) with 8 populated memory channel operating at 3200 MT/s.

**Question:**
- what is the aggregated CPU memory bandwith per socket in gigabytes per second ?

## Know your GPU

Buid and run `deviceQuery.cpp` (directly copied from Nvidia's [CUDA samples](https://github.com/NVIDIA/cuda-samples/blob/master/Samples/1_Utilities/deviceQuery/deviceQuery.cpp):


```shell
module load nvidia/cuda/12.0

nvcc -I. deviceQuery.cpp -o deviceQuery
```

Run `deviceQuery` on the gpua30 partition. Cross-check that there are effectively 4 Nvidia GPU of type A30.

**Question:**
- extract information `Memory Bus Width` and `Memory Clock rate` to compute memory bandwith (don't forget to multiply by to take into account Read and Write memory transfers).
- what is the the ratio of memory bandwith between one CPU socket and one A30 GPU ?


## Explore memory bandwith with saxpy

Use code locate in sub-folder saxpy to evaluate memory bandwith, and see which fraction of the theoretical memory bandwith one can reach.

## Going (much) further on analyzing compute performance

See slides from the [Node-level Performance Engineering](https://moodle.nhr.fau.de/course/view.php?id=55) training at HLRS center.
