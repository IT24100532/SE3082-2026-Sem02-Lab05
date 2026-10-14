#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000000
#ifndef STRIP
#define STRIP 1024  /* multiple of every SIMD width (4, 8 or 16 floats); one strip of A, B and C fits in L1 cache */
#endif
#define REPEATS 1000 /* repeat the multiplication so the time is measurable */

int main() {
  float *A = malloc(N * sizeof(float));
  float *B = malloc(N * sizeof(float));
  float *C = malloc(N * sizeof(float));
  double tstart, tserial, tparallel;

  for (int i = 0; i < N; i++) {
    A[i] = i * 0.5f;
    B[i] = 2.0f;
  }

  /* Serial reference */
  tstart = omp_get_wtime();
  for (int rep = 0; rep < REPEATS; rep++)
    for (int i = 0; i < N; i++)
      C[i] = A[i] * B[i];
  tserial = omp_get_wtime() - tstart;
  double serial_sum = 0.0;
  for (int i = 0; i < N; i++) serial_sum += C[i];

  /* Strip mining: outer loop walks over strips (shared between threads),
     inner loop handles one strip that fits a SIMD vector */
  tstart = omp_get_wtime();
  for (int rep = 0; rep < REPEATS; rep++) {
    #pragma omp parallel for schedule(static)
    for (int is = 0; is < N; is += STRIP) {
      int end = (is + STRIP < N) ? is + STRIP : N;
      #pragma omp simd
      for (int i = is; i < end; i++)
        C[i] = A[i] * B[i];
    }
  }
  tparallel = omp_get_wtime() - tstart;
  double parallel_sum = 0.0;
  for (int i = 0; i < N; i++) parallel_sum += C[i];

  printf("N = %d, strip size = %d, threads = %d, repeats = %d\n", N, STRIP, omp_get_max_threads(), REPEATS);
  printf("serial:       time = %f s, checksum = %.1f\n", tserial, serial_sum);
  printf("strip-mined:  time = %f s, checksum = %.1f\n", tparallel, parallel_sum);
  printf("results %s, speedup = %.2fx\n", serial_sum == parallel_sum ? "match" : "DIFFER", tserial / tparallel);

  free(A); free(B); free(C);
  return 0;
}
