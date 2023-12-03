EXE_NAME = "saxpy"

default: build
	@echo "Start Build"

CXX ?= g++
CXXFLAGS ?= -O3 -mavx -fopenmp

SRC = $(wildcard *.cpp)
EXE_FLOAT = ${EXE_NAME}.float.host
EXE_DOUBLE = ${EXE_NAME}.double.host

build: $(EXE_FLOAT) $(EXE_DOUBLE)

$(EXE_FLOAT): $(SRC)
	$(CXX) -DUSE_FLOAT $(CXXFLAGS) $(SRC) -o $@

$(EXE_DOUBLE): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $@

clean:
	rm -f *.o *.host

# Compilation rules
%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $<
