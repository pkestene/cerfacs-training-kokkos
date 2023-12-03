# ######## GNU ########
F90 = gfortran
FFLAGS = -O3 -Wall -Wextra -cpp -ffixed-line-length-none -ffree-line-length-none
LDFLAGS = -O3

SRCDIR = .

SRC = \
	m_random_utils.F90 \
	m_compute_kernel.F90 \
	main.F90

OBJS = $(SRC:.F90=.o)

all: saxpy

saxpy: $(OBJS)
	$(F90) $(LDFLAGS_CUDA) $(OBJS) -o $@

clean:
	rm -f *.o *.mod saxpy saxpy

cleanall: clean

%.o:    $(SRCDIR)/%.F90
	$(F90) $(FFLAGS) -c $<
