/* Area of the Mandelbrot set, parallelised with OpenMP (based on the provided area.c).
   1. A parallel region starts before the main loop; private, shared and reduction
      variables are declared on it.
   2. The outer loop is split by hand with omp_get_thread_num() / omp_get_num_threads(),
      so every thread gets an equal number of points. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

# define NPOINTS 2000
# define MAXITER 2000


struct complex{
  double real;
  double imag;
};

int main(){
  int i, j, iter, numoutside = 0;
  double area, error, ztemp;
  struct complex z, c;
  int id, nthreads, istart, iend, team = 1;
  double tstart, tcalc;

  tstart = omp_get_wtime();

#pragma omp parallel default(none) private(i, j, iter, ztemp, z, c, id, nthreads, istart, iend) \
                     shared(team) reduction(+:numoutside)
  {
    id = omp_get_thread_num();
    nthreads = omp_get_num_threads();
    istart = id * NPOINTS / nthreads;          /* this thread's equal share of i */
    iend = (id + 1) * NPOINTS / nthreads;

#pragma omp single
    team = nthreads;

    for (i=istart; i<iend; i++) {
      for (j=0; j<NPOINTS; j++) {
        c.real = -2.0+2.5*(double)(i)/(double)(NPOINTS)+1.0e-7;
        c.imag = 1.125*(double)(j)/(double)(NPOINTS)+1.0e-7;
        z=c;
        for (iter=0; iter<MAXITER; iter++){
          ztemp=(z.real*z.real)-(z.imag*z.imag)+c.real;
          z.imag=z.real*z.imag*2+c.imag;
          z.real=ztemp;
          if ((z.real*z.real+z.imag*z.imag)>4.0e0) {
            numoutside++;
            break;
          }
        }
      }
    }
  }

  tcalc = omp_get_wtime() - tstart;

      area=2.0*2.5*1.125*(double)(NPOINTS*NPOINTS-numoutside)/(double)(NPOINTS*NPOINTS);
      error=area/(double)NPOINTS;

      printf("threads = %d: Area of Mandlebrot set = %12.8f +/- %12.8f, time = %f s\n",team,area,error,tcalc);

  return 0;
}
