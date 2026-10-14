#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define CUTOFF 20  /* below this, tasks cost more than they save, so run serially */

int fib_serial(int n) {
  int i, j;
  if (n<2)
    return n;
  else {
    i=fib_serial(n-1);
    j=fib_serial(n-2);
    return i+j;
  }
}

int fib(int n) {
  int i, j;
  if (n<2)
    return n;
  if (n < CUTOFF)
    return fib_serial(n);

  #pragma omp task shared(i)
  i=fib(n-1);
  #pragma omp task shared(j)
  j=fib(n-2);
  #pragma omp taskwait   /* wait for both child tasks before using i and j */
  return i+j;
}

int main(int argc, char *argv[]) {
  int n = (argc > 1) ? atoi(argv[1]) : 40;
  int result;
  double tstart, tstop;

  tstart = omp_get_wtime();
  result = fib_serial(n);
  tstop = omp_get_wtime();
  printf("serial:   fib(%d) = %d, time = %f s\n", n, result, tstop - tstart);

  tstart = omp_get_wtime();
  #pragma omp parallel
  {
    #pragma omp single   /* one thread creates the first task, the rest pick tasks up */
    result = fib(n);
  }
  tstop = omp_get_wtime();
  printf("parallel: fib(%d) = %d, time = %f s, threads = %d\n", n, result, tstop - tstart,
         omp_get_max_threads());

  return 0;
}
