/* Extra exercise: different ways of mapping iterations to threads (based on the provided area.c).
   The outer loop uses schedule(runtime), so OMP_SCHEDULE chooses the mapping,
   e.g. OMP_SCHEDULE="static,1" or "dynamic,10". */
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
  double tstart, tcalc;

  tstart = omp_get_wtime();

#pragma omp parallel for default(none) private(j, iter, ztemp, z, c) \
                         reduction(+:numoutside) schedule(runtime)
  for (i=0; i<NPOINTS; i++) {
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

  tcalc = omp_get_wtime() - tstart;

      area=2.0*2.5*1.125*(double)(NPOINTS*NPOINTS-numoutside)/(double)(NPOINTS*NPOINTS);
      error=area/(double)NPOINTS;

      printf("threads = %d: Area of Mandlebrot set = %12.8f +/- %12.8f, time = %f s\n",omp_get_max_threads(),area,error,tcalc);

  return 0;
}
