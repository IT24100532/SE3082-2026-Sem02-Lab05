# Builds every exercise. Linux (EC2): gcc -fopenmp. macOS: Apple clang + Homebrew libomp.
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

PROGS := Exercise01/exercise01 Exercise02/exercise02 Exercise03/exercise03 \
         Exercise04/exercise04 Exercise04/exercise04_collapse Exercise05/exercise05 \
         Exercise06/exercise06 Exercise07/area Exercise07/area_parallel \
         Exercise07/area_schedule

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(OMPFLAGS)

clean:
	rm -f $(PROGS)
