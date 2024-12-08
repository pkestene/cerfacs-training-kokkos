# you have set CUDA_PATH env variable either manually
# or using "module load cuda"
CUDA_PATH ?= /usr/local/cuda
CUDA_INC_PATH=${CUDA_PATH}/include
CUDA_LIB_PATH=${CUDA_PATH}/lib64
CUDA_ARCH ?= 90

# calypso-grace node:
# module load gcc/12.3.0_arm nvidia/cuda/12.4
CC=nvcc
CPU_FLAGS=-Xcompiler -mcpu=neoverse-n2 -Xcompiler -msve-vector-bits=128
CFLAGS= -O3 -I$(CUDA_INC_PATH) --ptxas-options=-v -arch=sm_${CUDA_ARCH} -lineinfo ${CPU_FLAGS} -Xcompiler -fopenmp
LDFLAGS=-L$(CUDA_LIB_PATH) -lcudart -lcublas -lgomp

# module load nvhpc/24.7
#CC=nvc++
#CFLAGS= -fast -Minfo=mp -mp=multicore -tp=neoverse-v2
#LDFLAGS=-L${NVHPC_ROOT}/math_libs/12.5/lib64 -lcublas

all: saxpy.float.$(CC) saxpy.double.$(CC)

saxpy.float.$(CC): saxpy.cu HostTimer.h CudaTimer.h my_cuda_utils.cu my_cuda_utils.h
	$(CC) $(CFLAGS) -DUSE_FLOAT $(LDFLAGS) my_cuda_utils.cu  saxpy.cu -o $@

saxpy.double.$(CC): saxpy.cu HostTimer.h CudaTimer.h my_cuda_utils.cu my_cuda_utils.h
	$(CC) $(CFLAGS) $(LDFLAGS) my_cuda_utils.cu  saxpy.cu -o $@

clean:
	\rm -f *.o
	\rm -f saxpy.float* saxpy.double*
	\rm -f *~
	\rm -f *.linkinfo *.log
