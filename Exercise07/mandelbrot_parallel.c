/* Area of the Mandelbrot set (OpenMP).
   A parallel region is started before the main loop, and the outer loop is split by hand
   with the OpenMP library routines so every thread gets an equal number of points. */
#include <omp.h>
#include <stdio.h>

#define NPOINTS 2000
#define MAXITER 2000

int main() {
  int numoutside = 0;
  int nthreads = 1;
  double tstart = omp_get_wtime();

  #pragma omp parallel default(none) shared(nthreads) reduction(+:numoutside)
  {
    int id = omp_get_thread_num();
    int nt = omp_get_num_threads();
    /* Equal share of the outer loop for this thread */
    int istart = id * NPOINTS / nt;
    int iend = (id + 1) * NPOINTS / nt;

    #pragma omp single
    nthreads = nt;

    for (int i = istart; i < iend; i++) {
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
  }

  double tcalc = omp_get_wtime() - tstart;
  double area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS * NPOINTS - numoutside) / (double)(NPOINTS * NPOINTS);
  printf("threads = %d: area = %.8f, points outside = %d, time = %f s\n", nthreads, area, numoutside, tcalc);
  return 0;
}
