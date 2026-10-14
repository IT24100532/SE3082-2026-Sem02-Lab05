/* Extra exercise: different ways of mapping iterations to threads.
   The schedule is chosen at run time with OMP_SCHEDULE (e.g. "static", "static,1", "dynamic,10", "guided"). */
#include <omp.h>
#include <stdio.h>

#define NPOINTS 2000
#define MAXITER 2000

int main() {
  int numoutside = 0;
  double tstart = omp_get_wtime();

  #pragma omp parallel for schedule(runtime) reduction(+:numoutside)
  for (int i = 0; i < NPOINTS; i++) {
    for (int j = 0; j < NPOINTS; j++) {
      double cr = -2.0 + 2.5 * (double)i / NPOINTS + 1.0e-7;
      double ci = 1.125 * (double)j / NPOINTS + 1.0e-7;
      double zr = cr, zi = ci;
      for (int iter = 0; iter < MAXITER; iter++) {
        double ztemp = zr * zr - zi * zi + cr;
        zi = 2.0 * zr * zi + ci;
        zr = ztemp;
        if (zr * zr + zi * zi > 4.0) {
          numoutside++;
          break;
        }
      }
    }
  }

  double tcalc = omp_get_wtime() - tstart;
  double area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS * NPOINTS - numoutside) / (double)(NPOINTS * NPOINTS);
  printf("threads = %d: area = %.8f, points outside = %d, time = %f s\n", omp_get_max_threads(), area, numoutside, tcalc);
  return 0;
}
