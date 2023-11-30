#COMPILER = gnu
#CXX = g++
#CXXFLAGS = -Ofast -fopenmp -foffload="-march=nvptx-non -lm"

#COMPILER = clang
#CXX = clang++
#CXXFLAGS = -O3 -fopenmp=libomp -fopenmp-targets=nvptx64 --offload-arch=sm_80

COMPILER = nvhpc
CXX = nvc++
CXXFLAGS = -O3 -Minfo=mp -mp=gpu

dot_prod: dot_prod.cpp
	$(CXX) $(CXXFLAGS) $< -o $@.$(COMPILER)
