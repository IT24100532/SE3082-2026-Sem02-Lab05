/* Area of the Mandelbrot set (serial).
   Written from the description in the lab sheet, because the provided zip was not available. */
#include <omp.h>
#include <stdio.h>

#define NPOINTS 2000
#define MAXITER 2000

int main() {
  int numoutside = 0;
  double tstart = omp_get_wtime();

  /* Grid over the box [-2, 0.5] x [0, 1.125]: the upper half of the set */
  for (int i = 0; i < NPOINTS; i++) {
    for (int j = 0; j < NPOINTS; j++) {
      double cr = -2.0 + 2.5 * (double)i / NPOINTS + 1.0e-7;
      double ci = 1.125 * (double)j / NPOINTS + 1.0e-7;
      double zr = cr, zi = ci;
      for (int iter = 0; iter < MAXITER; iter++) {
        double ztemp = zr * zr - zi * zi + cr;
        zi = 2.0 * zr * zi + ci;
        zr = ztemp;
        if (zr * zr + zi * zi > 4.0) {  /* |z| > 2 */
          numoutside++;
          break;
        }
      }
    }
  }

  double tcalc = omp_get_wtime() - tstart;
  double area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS * NPOINTS - numoutside) / (double)(NPOINTS * NPOINTS);
  printf("serial: area = %.8f, points outside = %d, time = %f s\n", area, numoutside, tcalc);
  return 0;
}
