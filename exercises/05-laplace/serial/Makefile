CPPFLAGS = -DUSE_DOUBLE -fopenmp

CC=g++

all: run

laplace2d: main.cpp params.h common.h DataContext.h laplace2d_serial_kernel.h HostTimer.h
	$(CC) -Wall -Wextra $(CPPFLAGS) main.cpp -o $@

run: laplace2d
	./laplace2d

clean:
	rm -rf laplace2d *.o
