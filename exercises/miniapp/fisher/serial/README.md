# Reaction-Diffusion - Fisher Equation

This code is adapted from https://github.com/eth-cscs/SummerSchool2016

To compile and run the serial version of the miniapp

```bash
# choose your preferred version (C++ in this example)
cd cxx

# make the code
make

# now run on a 128x128 grid to t=0.01 via 100 time steps
./main 128 128 100 0.01
```

Benchmark results on kraken `sbatch job.sh`

Typical number of CG iterations/second should be arround 10000.
