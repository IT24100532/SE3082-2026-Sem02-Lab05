# md_serial uses forces.c; md_omp uses forces_omp.c (OpenMP).
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
  LIBOMP := $(shell brew --prefix libomp)
  CC := clang
  OMPFLAGS := -Xclang -fopenmp -I$(LIBOMP)/include -L$(LIBOMP)/lib -lomp
else
  CC := gcc
  OMPFLAGS := -fopenmp
endif
CFLAGS := -O2 -Wall
COMMON := main.c fcc.c random.c mxwell.c domove.c mkekin.c velavg.c dscal.c prnout.c

all: md_serial md_omp

md_serial: $(COMMON) forces.c moldyn.h
	$(CC) $(CFLAGS) $(COMMON) forces.c -o $@ -lm

md_omp: $(COMMON) forces_omp.c moldyn.h
	$(CC) $(CFLAGS) $(COMMON) forces_omp.c -o $@ $(OMPFLAGS) -lm

clean:
	rm -f md_serial md_omp
