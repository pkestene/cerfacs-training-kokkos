# build with cmake on kraken

```shell
module load nvidia/cuda/12.0

mkdir -p _build
cd _build
cmake -DCMAKE_CUDA_ARCHITECTURES="80" ..
make

sbatch job.kraken
```
