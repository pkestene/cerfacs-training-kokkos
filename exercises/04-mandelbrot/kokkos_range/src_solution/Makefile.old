# KOKKOS setup (OpenMP or CUDA backend)
# here we assume KOKKOS_PATH is setup by env variable or command line
KOKKOS_PATH ?= .
include $(KOKKOS_PATH)/Makefile.kokkos

EXE_PREFIX=mandelbrot

ifneq (,$(findstring Cuda,$(KOKKOS_DEVICES)))
CXX = $(NVCC_WRAPPER)
CXXFLAGS = -ccbin g++ -I. -Wall -DCUDA
LDFLAGS = -lgomp
EXE = $(EXE_PREFIX).cuda
else
# Use either GNU or XLC compiler (for Power8)
CXX = g++
CXXFLAGS = -g -O3 -I. -Wall --std=c++11
#CXX = xlc++
#CXXFLAGS = -O5 -I. -qarch=pwr8 -qsmp=omp -std=c++11
EXE = $(EXE_PREFIX).omp
endif

SRCDIR = $(shell pwd)
SRC = \
	OpenMPTimer.cpp \
	main.cpp

TMP_OBJ = $(SRC:.c=.o)
OBJ     = $(TMP_OBJ:.cpp=.o)

.DEFAULT_GOAL=$(EXE)

$(EXE) : $(OBJ)
	echo $@
	$(CXX) $(KOKKOS_LDFLAGS) $(LDFLAGS) $(EXTRA_PATH) $(OBJ) $(KOKKOS_LIBS) $(LIB) -o $@

clean:
	rm -f $(OBJ) $(EXE)

cleandata:
	rm -f *.ppm

cleanall: clean
	rm -f *.ppm

%.o:    %.cpp $(KOKKOS_CPP_DEPENDS)
	$(CXX) $(KOKKOS_CPPFLAGS) $(KOKKOS_CXXFLAGS) $(CXXFLAGS) $(EXTRA_INC) -c $< -o $@
