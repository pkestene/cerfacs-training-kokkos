# you have set CUDA_PATH env variable either manually
# or using "module load cuda"
CUDA_PATH ?= /usr/local/cuda
CUDA_INC_PATH=${CUDA_PATH}/include
CUDA_LIB_PATH=${CUDA_PATH}/lib64
CUDA_ARCH ?= 80

CC=nvcc
CFLAGS= -O3 -I$(CUDA_INC_PATH) --ptxas-options=-v -arch=sm_${CUDA_ARCH} -lineinfo -Xcompiler -fopenmp
LDFLAGS=-L$(CUDA_LIB_PATH) -lcudart -lcublas -lgomp

all: saxpy.float saxpy.double

saxpy.float: saxpy.cu HostTimer.h CudaTimer.h my_cuda_utils.cu my_cuda_utils.h
	$(CC) $(CFLAGS) -DUSE_FLOAT $(LDFLAGS) my_cuda_utils.cu  saxpy.cu -o $@

saxpy.double: saxpy.cu HostTimer.h CudaTimer.h my_cuda_utils.cu my_cuda_utils.h
	$(CC) $(CFLAGS) $(LDFLAGS) my_cuda_utils.cu  saxpy.cu -o $@

clean:
	\rm -f *.o
	\rm -f saxpy
	\rm -f *~
	\rm -f *.linkinfo *.log
